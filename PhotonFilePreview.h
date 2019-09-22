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
 * by bn on 01/07/2018.
 */
class PhotonFilePreview {
private:
	int32_t resolutionX_;
	int32_t resolutionY_;
	int32_t imageAddress_;
	int32_t dataSize_;

    std::vector<char> rawImageData_;

    std::vector<int32_t> imageData_;

	int32_t p1_;
	int32_t p2_;
	int32_t p3_;
	int32_t p4_;

public:
	PhotonFilePreview(int previewAddress, const std::string & fileContent) {
        std::string data(fileContent.begin() + previewAddress, fileContent.begin() + previewAddress + 32);
		std::stringstream ss(data);
        PhotonInputStream ds(ss);

        resolutionX_ = ds.readInt();
        resolutionY_ = ds.readInt();
        imageAddress_ = ds.readInt();
        dataSize_ = ds.readInt();
        p1_ = ds.readInt();
        p2_ = ds.readInt();
        p3_ = ds.readInt();
        p4_ = ds.readInt();

        rawImageData_.assign(fileContent.begin() + imageAddress_, fileContent.begin() + imageAddress_ + dataSize_);

        decodeImageData();
    }

    void save(PhotonOutputStream os, int startAddress) {
        os.writeInt(resolutionX_);
        os.writeInt(resolutionY_);
        os.writeInt(startAddress + 4+4+4+4 + 4+4+4+4);
        os.writeInt(dataSize_);
        os.writeInt(p1_);
        os.writeInt(p2_);
        os.writeInt(p3_);
        os.writeInt(p4_);
        os.write(rawImageData_, 0, dataSize_);
    }

    int getByteSize() {
        return 4+4+4+4 + 4+4+4+4 + dataSize_;
    }

private:
	void decodeImageData() {
        imageData_.resize(resolutionX_ * resolutionY_);
        int d = 0;
        for (int i = 0; i < dataSize_; i++) {
			int32_t dot = (rawImageData_[i] & 0xFF) | ((rawImageData_[i+1] & 0xFF) << 8);
			i++;

			int32_t color =   ((dot & 0xF800) << 8) | ((dot & 0x07C0) << 5) | ((dot & 0x001F) << 3);

//            int red = ((dot >> 11) & 0x1F) << 3;
//            int green = ((dot >> 6) & 0x1F) << 3;
//            int blue = (dot & 0x1F) << 3;
//            color = red<<16 | green<<8 | blue;

            int repeat = 1;
            if ((dot & 0x0020) == 0x0020) {
                repeat += (rawImageData_[i+1] & 0xFF) | ((rawImageData_[i+2] & 0x0F) << 8);
				i += 2;
            }

            while (repeat > 0) {
                imageData_[d++] = color;
                repeat--;
            }
        }

    }

public:
	int32_t getResolutionX() {
        return resolutionX_;
    }

	int32_t getResolutionY() {
        return resolutionY_;
    }

    std::vector<int32_t> getImageData() {
        return imageData_;
    }

    void unLink() {
        rawImageData_.clear();
        imageData_.clear();
    }

};
