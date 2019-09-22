/*
 * MIT License
 *
 * Copyright (c) 2018 Bonosoft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/**
 * by bn on 02/07/2018.
 */
class PhotonLayer {
public:


private:
	int32_t width_;
	int32_t height_;
	int32_t islandCount_ = 0;

	std::vector<std::vector<char>> iArray;
	std::vector<int32_t> pixels;
	std::vector<int32_t> rowIslands;
	std::vector<int32_t> rowUnsupported;
	std::vector<int32_t> rowSupported;

public:
	PhotonLayer(int width, int height) {
        width_ = width;
        height_ = height;

		for (int h = 0; h < height; h++)
			iArray.push_back({});

		for (int h = 0; h < height; h++)
			iArray[h].resize(width);

        pixels.resize(height);
        rowIslands.resize(height);
        rowUnsupported.resize(height);
        rowSupported.resize(height);
    }

    void clear() {
        for (int y = 0; y < height_; y++) {
            for (int x = 0; x < width_; x++) {
                iArray[y][x] = PhotonDot::OFF;
            }
        }
		std::fill(pixels.begin(), pixels.end(), 0);
		std::fill(rowIslands.begin(), rowIslands.end(), 0);
		std::fill(rowUnsupported.begin(), rowUnsupported.end(), 0);
		std::fill(rowSupported.begin(), rowSupported.end(), 0);
    }

    void supported(int x, int y) {
        iArray[y][x] = PhotonDot::SUPPORTED;
        rowSupported[y]++;
        pixels[y]++;
    }

    void unSupported(int x, int y) {
        iArray[y][x] = PhotonDot::CONNECTED;
        rowUnsupported[y]++;
        pixels[y]++;
    }

    void island(int x, int y) {
        iArray[y][x] = PhotonDot::ISLAND;
        rowIslands[y]++;
        islandCount_++;
        pixels[y]++;
    }

    void remove(int x, int y, char type) {
        iArray[y][x] = PhotonDot::OFF;
        switch (type) {
            case PhotonDot::SUPPORTED:
                rowUnsupported[y]--;
                break;
            case PhotonDot::CONNECTED:
                rowUnsupported[y]--;
                break;
            case PhotonDot::ISLAND:
                rowIslands[y]--;
                islandCount_--;
                break;
        }
        pixels[y]--;
    }


    void reduce() {
        // Double reduce to handle single line connections.
        for (int i = 0; i < 2; i++) {
            if (islandCount_ > 0) {
                for (int y = 0; y < height_; y++) {
                    if (rowIslands[y] > 0) {
                        for (int x = 0; x < width_; x++) {
                            if (iArray[y][x] == PhotonDot::ISLAND) {
                                if (connected(x, y)) {
                                    makeConnected(x, y);
                                    checkUp(x, y);
                                    if (rowIslands[y] == 0) {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

	private:
	void checkUp(int x, int y) {
        if (y > 0 && rowIslands[y - 1] > 0 && iArray[y - 1][x] == PhotonDot::ISLAND) {
            makeConnected(x, y - 1);
            checkUp(x, y - 1);
        }
        if (x > 0 && rowIslands[y] > 0 && iArray[y][x - 1] == PhotonDot::ISLAND) {
            makeConnected(x - 1, y);
            checkBackUp(x - 1, y);
        }
        if (x < (width_-1) && rowIslands[y] > 0 && iArray[y][x + 1] == PhotonDot::ISLAND) {
            makeConnected(x + 1, y);
            checkFrontUp(x + 1, y);
        }
    }

    void checkBackUp(int x, int y) {
        if (y > 0 && rowIslands[y - 1] > 0 && iArray[y - 1][x] == PhotonDot::ISLAND) {
            makeConnected(x, y - 1);
            checkBackUp(x, y - 1);
        }
        if (x > 0 && rowIslands[y] > 0 && iArray[y][x - 1] == PhotonDot::ISLAND) {
            makeConnected(x - 1, y);
            checkBackUp(x - 1, y);
        }
    }

    void checkFrontUp(int x, int y) {
        if (y > 0 && rowIslands[y - 1] > 0 && iArray[y - 1][x] == PhotonDot::ISLAND) {
            makeConnected(x, y - 1);
            checkFrontUp(x, y - 1);
        }
        if (x < (width_-1) && rowIslands[y] > 0 && iArray[y][x + 1] == PhotonDot::ISLAND) {
            makeConnected(x + 1, y);
            checkFrontUp(x + 1, y);
        }
    }

    void makeConnected(int x, int y) {
        iArray[y][x] = PhotonDot::CONNECTED;
        rowSupported[y]++;
        rowIslands[y]--;
        islandCount_--;
    }

    bool connected(int x, int y) {
        return (x > 0 && (iArray[y][x - 1] & 0x01) == PhotonDot::SUPPORTED)
                || (x < (width_ - 1) && (iArray[y][x + 1] & 0x01) == PhotonDot::SUPPORTED)
                || (y > 0 && (iArray[y - 1][x] & 0x01) == PhotonDot::SUPPORTED)
                || (y < (height_ - 1) && (iArray[y + 1][x] & 0x01) == PhotonDot::SUPPORTED);
    }

	public:
	int setIslands(std::vector<photoncpp::BitSet> & islandRows) {
        int islands = 0;
        for (int y = 0; y < height_; y++) {
			photoncpp::BitSet bitSet;
            if (rowIslands[y] > 0) {
                for (int x = 0; x < width_; x++) {
                    if (iArray[y][x] == PhotonDot::ISLAND) {
						bitSet[x] = true;
                    }
                }
            }
            islandRows.push_back(bitSet);
            islands += rowIslands[y];
        }
        return islands;
    }

    void unLink() {
        iArray.clear();
        pixels.clear();
        rowIslands.clear();
        rowUnsupported.clear();
        rowSupported.clear();
    }

    std::string packLayerImage() {
		std::stringstream baos;

		{
            for (int y = 0; y < height_; y++) {
                if (pixels[y] == 0) {
                    add(baos, PhotonDot::OFF, width_);
                } else {
                    char current = PhotonDot::OFF;
                    int length = 0;
                    for (int x = 0; x < width_; x++) {
						char next = iArray[y][x];
                        if (next != current) {
                            if (length > 0) {
                                add(baos, current, length);
                            }
                            current = next;
                            length = 1;
                        } else {
                            length++;
                        }
                    }
                    if (length > 0) {
                        add(baos, current, length);
                    }
                }
            }

			return baos.str();
        }
    }

    void unpackLayerImage(std::string packedLayerImage) {
        clear();
        int x = 0;
        int y = 0;
        for (size_t i = 0; i < packedLayerImage.length(); i++) {
            char rle = packedLayerImage[i];
            char colorCode = (char) ((rle & 0x60) >> 5);

            bool extended = (rle & 0x80) == 0x80;
            int length = rle & 0x1F;
            if (extended) {
                i++;
                length = (length << 8) | (packedLayerImage[i] & 0x00ff);
            }

            for(int xi = x; xi<(x+length); xi++) {
                switch (colorCode) {
                    case PhotonDot::SUPPORTED:
                        supported(xi, y);
                        break;
                    case PhotonDot::CONNECTED:
                        unSupported(xi, y);
                        break;
                    case PhotonDot::ISLAND:
                        island(xi, y);
                        break;
                }
            }
            x += length;
            if (x >= width_) {
                y++;
                x = 0;
            }
        }

    }

	private:
	void add(std::ostream & baos, char current, int length) {
        if (length < 32) {
			std::vector<char> data(1, 0);
            data[0] = (char) ((current << 5) | (length & 0x1f));
            baos.write(data.data(), data.size());
        } else {
			std::vector<char> data(2, 0);
            data[0] = (char) (0x80 | (current << 5) | (length >> 8 & 0x00FF));
            data[1] = (char) (length & 0x00FF);
            baos.write(data.data(), data.size());
        }
    }

    /**
     * Get a layer image for drawing.
     * <p/>
     * This will decode the RLE packed layer information and return a list of rows, with color and length information
     *
     * @param packedLayerImage The packed layer image information
     * @param width            The width of the current layer, used to change rows
     * @return A list with the
     */
	public:
	static std::vector<PhotonRow> getRows(std::string packedLayerImage, int width, bool isCalculated) {
        std::map<char, photoncpp::Color> colors;
        colors[PhotonDot::OFF] = 0x000000;
        if (isCalculated) {
            colors[PhotonDot::SUPPORTED] = 0x008800;
        } else {
            colors[PhotonDot::SUPPORTED] = 0x000088;
        }
        colors[PhotonDot::CONNECTED] = 0xFFFF00;
        colors[PhotonDot::ISLAND] = 0xFF0000;
		std::vector<PhotonRow> rows;
        int resolutionX_ = width - 1;
        PhotonRow currentRow;
        rows.push_back(currentRow);
        int x = 0;
        if (!packedLayerImage.empty()) { // when user tries to show a layer before its calculated
            for (size_t i = 0; i < packedLayerImage.length(); i++) {
                char rle = packedLayerImage[i];
				char colorCode = (char) ((rle & 0x60) >> 5);
				photoncpp::Color color = colors[colorCode];
                bool extended = (rle & 0x80) == 0x80;
                int length = rle & 0x1F;
                if (extended) {
                    i++;
                    length = (length << 8) | (packedLayerImage[i] & 0x00ff);
                }
                currentRow.lines_.push_back(PhotonLine(color, length));
                x += length;

                if (x >= resolutionX_) {
                    currentRow.lines_.clear(); // GLS: what is "current"?
					rows.push_back(currentRow);
                    x = 0;
                }
            }
        }
        return rows;
    }

    int fixlayer() {
        PhotonMatix photonMatix;
		std::vector<PhotonDot> dots;
        if (islandCount_ > 0) {
            for (int y = 0; y < height_; y++) {
                if (rowIslands[y] > 0) {
                    for (int x = 0; x < width_; x++) {
                        if (iArray[y][x] == PhotonDot::ISLAND) {
                            photonMatix.clear();
                            int blanks = photonMatix.set(x, y, iArray, width_, height_);
                            if (blanks>0) { // one or more neighbour pixels are OFF
                                photonMatix.calc();
                                photonMatix.level();
                                photonMatix.calc();

                                for(int ry=0; ry<3; ry++) {
                                    for (int rx = 0; rx < 3; rx++) {
                                        int iy = y-1+ry;
                                        int ix = x-1+rx;
                                        if (iArray[iy][ix] == PhotonDot::OFF) {
                                            if (photonMatix.calcMatrix_[1+ry][1+rx]>3) {
                                                dots.push_back(PhotonDot(ix, iy));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        for(PhotonDot dot : dots) {
            island(dot.x_, dot.y_);
        }
        return dots.size();
    }

    std::string packImageData() {
		std::stringstream baos;
		
		{
            for (int y = 0; y < height_; y++) {
                if (pixels[y] == 0) {
                    addPhotonRLE(baos, true, width_);
                } else {
                    char current = PhotonDot::OFF;
                    int length = 0;
                    for (int x = 0; x < width_; x++) {
						char next = iArray[y][x];
                        if (next != current) {
                            if (length > 0) {
                                addPhotonRLE(baos, current == PhotonDot::OFF, length);
                            }
                            current = next;
                            length = 1;
                        } else {
                            length++;
                        }
                    }
                    if (length > 0) {
                        addPhotonRLE(baos, current == PhotonDot::OFF, length);
                    }
                }
            }

            return baos.str();
        }
    }

	private:
	void addPhotonRLE(std::ostream & baos, bool off, int length) {
		std::vector<char> data({ 0 });
        while (length>0) {
            int lineLength = std::min(length, 125); // max storage length of 0x7D (125) ?? Why not 127?
            data[0] = (char) ((off ? 0x00: 0x80) | (lineLength & 0x7f));
            baos.write(data.data(), data.size());
            length -= lineLength;
        }
    }

	public:
	char get(int x, int y) {
        return iArray[y][x];
    }

};
