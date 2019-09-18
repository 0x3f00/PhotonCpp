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
class PhotonFileLayer {
private:
	float layerPositionZ_;
    float layerExposure_;
    float layerOffTimeSeconds_;
    int32_t dataAddress_;
	int32_t dataSize_;
	int32_t unknown1_;
	int32_t unknown2_;
	int32_t unknown3_;
	int32_t unknown4_;

    std::string imageData_;

    std::string packedLayerImage_;

    std::vector<photoncpp::BitSet> islandRows_;
	int32_t isLandsCount_;
    uint64_t pixels_;

	std::vector<PhotonFileLayer> antiAliasLayers_;

    bool extendsMargin_;
//    PhotonFileHeader photonFileHeader_;
	int32_t width_;
	int32_t height_;
    bool isCalculated_;

//    PhotonFileLayer(PhotonInputStream & ds, PhotonFileHeader & photonFileHeader) : photonFileHeader_(photonFileHeader) {
public:
	PhotonFileLayer() = default;
//	PhotonFileLayer(const PhotonFileLayer &) = default;
	PhotonFileLayer(PhotonInputStream & ds, int32_t width, int32_t height) {
		width_ = width;
		height_ = height;
		layerPositionZ_ = ds.readFloat();
        layerExposure_ = ds.readFloat();
        layerOffTimeSeconds_ = ds.readFloat();

        dataAddress_ = ds.readInt();
        dataSize_ = ds.readInt();

        unknown1_ = ds.readInt();
        unknown2_ = ds.readInt();
        unknown3_ = ds.readInt();
        unknown4_ = ds.readInt();
    }

	PhotonFileLayer(PhotonFileLayer & photonFileLayer, int32_t width, int32_t height) {
		width_ = width;
		height_ = height;
        layerPositionZ_ = photonFileLayer.layerPositionZ_;
        layerExposure_ = photonFileLayer.layerExposure_;
        layerOffTimeSeconds_ = photonFileLayer.layerOffTimeSeconds_;
        dataAddress_ = photonFileLayer.dataAddress_;
        dataAddress_ = photonFileLayer.dataSize_;

        

        // Dont copy data, we are building new AA layers anyway
        //imageData = copy();
        //packedLayerImage = copy();
    }

    int savePos(int dataPosition) {
        dataAddress_ = dataPosition;
        return dataPosition + dataSize_;
    }

    void save(PhotonOutputStream & os) {
        os.writeFloat(layerPositionZ_);
        os.writeFloat(layerExposure_);
        os.writeFloat(layerOffTimeSeconds_);

        os.writeInt(dataAddress_);
        os.writeInt(dataSize_);

        os.writeInt(unknown1_);
        os.writeInt(unknown2_);
        os.writeInt(unknown3_);
        os.writeInt(unknown4_);
    }

    void saveData(PhotonOutputStream & os) {
        os.write(imageData_, 0, dataSize_);
    }

    static int getByteSize() {
        return 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4;
    }

	std::vector<photoncpp::BitSet> unpackImage(int resolutionX_) {
        pixels_ = 0;
        resolutionX_ = resolutionX_ - 1;
		std::vector<photoncpp::BitSet> unpackedImage;
		photoncpp::BitSet currentRow; 
        int x = 0;
        for (char rle : imageData_) {
            int length = rle & 0x7F;
            bool color = (rle & 0x80) == 0x80;
            if (color) {
                pixels_ += length;
            }
            int endPosition = x + (length - 1);
            int lineEnd = std::min(endPosition, resolutionX_);
            if (color) {
				currentRow.resize(1 + lineEnd);
                std::fill(currentRow.begin() + x, currentRow.begin() + 1 + lineEnd, true);
            }
            if (endPosition > resolutionX_) {
				unpackedImage.push_back(currentRow);
				currentRow.clear(); // GLS                
                lineEnd = endPosition - (resolutionX_ + 1);
                if (color) {
					currentRow.resize(1 + lineEnd);
                    std::fill(currentRow.begin(), currentRow.begin() + 1 + lineEnd, true);
                }
            }
            x = lineEnd + 1;
            if (x > resolutionX_) {
				unpackedImage.push_back(currentRow);
				currentRow.clear();// GLS = new BitSet();                
                x = 0;
            }
        }
		if(0 != x)
			unpackedImage.push_back(currentRow);
        return unpackedImage;
    }

    void aaPixels(std::vector<photoncpp::BitSet> & unpackedImage, PhotonLayer & photonLayer) {
        photonLayer.clear();

        for (size_t y = 0; y < unpackedImage.size(); y++) {
			photoncpp::BitSet & currentRow = unpackedImage.at(y);
            if (!currentRow.empty()) {
                for (size_t x = 0; x < currentRow.size(); x++) {
                    if (currentRow.at(x)) {
                        photonLayer.unSupported(x, y);
                    }
                }
            }
        }
    }

    void unknownPixels(std::vector<photoncpp::BitSet> & unpackedImage, PhotonLayer & photonLayer) {
        photonLayer.clear();

        for (size_t y = 0; y < unpackedImage.size(); y++) {
			photoncpp::BitSet & currentRow = unpackedImage.at(y);
            if (!currentRow.empty()) {
                for (size_t x = 0; x < currentRow.size(); x++) {
                    if (currentRow.at(x)) {
                        photonLayer.supported(x, y);
                    }
                }
            }
        }
    }

    void calculate(std::vector<photoncpp::BitSet> & unpackedImage, std::vector<photoncpp::BitSet> & previousUnpackedImage, PhotonLayer & photonLayer) {
        islandRows_;
        isLandsCount_ = 0;

        photonLayer.clear();

        for (size_t y = 0; y < unpackedImage.size(); y++) {
			photoncpp::BitSet & currentRow = unpackedImage.at(y);
			photoncpp::BitSet prevRow;
			if(!previousUnpackedImage.empty())
				prevRow = previousUnpackedImage.at(y);
            if (!currentRow.empty()) {
                for (size_t x = 0; x < currentRow.size(); x++) {
                    if (currentRow.at(x)) {
                        if (prevRow.empty() || prevRow.at(x)) {
                            photonLayer.supported(x, y);
                        } else {
                            photonLayer.island(x, y);
                        }
                    }
                }
            }
        }

        photonLayer.reduce();

        isLandsCount_ = photonLayer.setIslands(islandRows_);
    }


    static std::vector<PhotonFileLayer> readLayers(const PhotonFileHeader & photonFileHeader_, const std::string & fileContent, int margin/*, IPhotonProgress & iPhotonProgress*/) {
        PhotonLayer photonLayer(photonFileHeader_.getResolutionX(), photonFileHeader_.getResolutionY());

		std::vector<PhotonFileLayer> layers;

        int antiAliasLevel = 1;
        if (photonFileHeader_.getVersion() > 1) {
            antiAliasLevel = photonFileHeader_.getAntiAliasingLevel();
        }

        int layerCount = photonFileHeader_.getNumberOfLayers();

		std::string data(fileContent.begin() + photonFileHeader_.getLayersDefinitionOffsetAddress(), fileContent.end());
		std::stringstream ss(data);
		PhotonInputStream ds(ss);
		
		{
            std::map<int, PhotonFileLayer> layerMap;
            for (int i = 0; i < layerCount; i++) {

                //iPhotonProgress.showInfo("Reading photon file layer " + (i + 1) + "/" + photonFileHeader_.getNumberOfLayers());

                PhotonFileLayer layer(ds, photonFileHeader_.getResolutionX(), photonFileHeader_.getResolutionY());
                layer.imageData_.assign(fileContent.begin() + layer.dataAddress_, fileContent.begin() + layer.dataAddress_ + layer.dataSize_);
                layers.push_back(layer);
                layerMap[i] = layer;
            }

            if (antiAliasLevel > 1) {
                for (int a = 0; a < (antiAliasLevel - 1); a++) {
                    for (int i = 0; i < layerCount; i++) {
                        //iPhotonProgress.showInfo("Reading photon file AA " + (2 + a) + "/" + antiAliasLevel + " layer " + (i + 1) + "/" + photonFileHeader_.getNumberOfLayers());

                        PhotonFileLayer layer(ds, photonFileHeader_.getResolutionX(), photonFileHeader_.getResolutionY());
                        layer.imageData_.assign(fileContent.begin() + layer.dataAddress_, fileContent.begin() + layer.dataAddress_ + layer.dataSize_);

                        layerMap[i].addAntiAliasLayer(layer);
                    }
                }
            }
        }

        photonLayer.unLink();
        //System.gc();

        return layers;
    }

    void addAntiAliasLayer(const PhotonFileLayer & layer) {
        antiAliasLayers_.push_back(layer);
    }

    static void calculateAALayers(const PhotonFileHeader & photonFileHeader, std::vector<PhotonFileLayer> & layers, PhotonAaMatrix photonAaMatrix/*, IPhotonProgress & iPhotonProgress*/) {
        PhotonLayer photonLayer(photonFileHeader.getResolutionX(), photonFileHeader.getResolutionY());
		photoncpp::MatrixInt source;
		source.resize(photonFileHeader.getResolutionY());
		for (auto & line : source)
			line.resize(photonFileHeader.getResolutionX());

        int i = 0;
        for (PhotonFileLayer & layer : layers) {
			std::vector<photoncpp::BitSet> unpackedImage = layer.unpackImage(photonFileHeader.getResolutionX());

            //iPhotonProgress.showInfo("Calculating AA for photon file layer " + i + "/" + photonFileHeader.getNumberOfLayers());


            for (uint32_t y = 0; y < photonFileHeader.getResolutionY(); y++) {
                for (uint32_t x = 0; x < photonFileHeader.getResolutionX(); x++) {
                    source[y][x] = 0;
                }
            }

            for (size_t y = 0; y < unpackedImage.size(); y++) {
				photoncpp::BitSet currentRow = unpackedImage.at(y);
                if (!currentRow.empty()) {
                    for (size_t x = 0; x < currentRow.size(); x++) {
                        if (currentRow.at(x)) {
                            source[y][x] = 255;
                        }
                    }
                }
            }

            // Calc
			photoncpp::MatrixInt target (photonAaMatrix.calc(source));

            int aaTresholdDiff = 255 / photonFileHeader.getAntiAliasingLevel();
            int aaTreshold = 0;
            for (PhotonFileLayer aaFileLayer : layer.antiAliasLayers_) {
                photonLayer.clear();
                aaTreshold += aaTresholdDiff;

                for (uint32_t y = 0; y < photonFileHeader.getResolutionY(); y++) {
                    for (uint32_t x = 0; x < photonFileHeader.getResolutionX(); x++) {
                        if (target[y][x] >= aaTreshold) {
                            photonLayer.supported(x, y);
                        }
                    }
                }

                aaFileLayer.saveLayer(photonLayer);
            }

            i++;
        }
        photonLayer.unLink();
        //System.gc();

    }

    static void calculateLayers(const PhotonFileHeader & photonFileHeader, std::vector<PhotonFileLayer> & layers, int margin/*, IPhotonProgress iPhotonProgress*/) {
        PhotonLayer photonLayer(photonFileHeader.getResolutionX(), photonFileHeader.getResolutionY());
		std::vector<photoncpp::BitSet> previousUnpackedImage;
        int i = 0;
        for (PhotonFileLayer layer : layers) {
			std::vector<photoncpp::BitSet> unpackedImage = layer.unpackImage(photonFileHeader.getResolutionX());

            //iPhotonProgress.showInfo("Calculating photon file layer " + i + "/" + photonFileHeader.getNumberOfLayers());

            if (margin > 0) {
                layer.extendsMargin_ = layer.checkMagin(unpackedImage, margin);
            }

            layer.unknownPixels(unpackedImage, photonLayer);

            layer.calculate(unpackedImage, previousUnpackedImage, photonLayer);

            previousUnpackedImage.clear();

			previousUnpackedImage = unpackedImage;

            layer.packedLayerImage_ = photonLayer.packLayerImage();
            layer.isCalculated_ = true;

            if (photonFileHeader.getVersion() > 1) {
                for (PhotonFileLayer aaFileLayer : layer.antiAliasLayers_) {
					std::vector<photoncpp::BitSet> aaUnpackedImage = aaFileLayer.unpackImage(photonFileHeader.getResolutionX());
                    PhotonLayer aaPhotonLayer(photonFileHeader.getResolutionX(), photonFileHeader.getResolutionY());
                    aaFileLayer.unknownPixels(aaUnpackedImage, aaPhotonLayer);
                    aaFileLayer.packedLayerImage_ = aaPhotonLayer.packLayerImage();
                    aaFileLayer.isCalculated_ = false;
                }
            }

            i++;
        }
        photonLayer.unLink();
        //System.gc();
    }

    static void calculateLayers(const PhotonFileHeader & photonFileHeader, std::vector<PhotonFileLayer> & layers, int margin, int layerNo) {
        PhotonLayer photonLayer(photonFileHeader.getResolutionX(), photonFileHeader.getResolutionY());
		std::vector<photoncpp::BitSet> previousUnpackedImage;

        if (layerNo > 0) {
            previousUnpackedImage = layers.at(layerNo - 1).unpackImage(photonFileHeader.getResolutionX());
        }

        for (int i = 0; i < 2; i++) {
            PhotonFileLayer layer = layers.at(layerNo + i);
			std::vector<photoncpp::BitSet> unpackedImage = layer.unpackImage(photonFileHeader.getResolutionX());

            if (margin > 0) {
                layer.extendsMargin_ = layer.checkMagin(unpackedImage, margin);
            }

            layer.unknownPixels(unpackedImage, photonLayer);

            layer.calculate(unpackedImage, previousUnpackedImage, photonLayer);

            previousUnpackedImage.clear();

            previousUnpackedImage = unpackedImage;

            layer.packedLayerImage_ = photonLayer.packLayerImage();
            layer.isCalculated_ = true;

            i++;
        }
        photonLayer.unLink();
        //System.gc();
    }

	std::vector<PhotonRow> getRows() {
        return PhotonLayer::getRows(packedLayerImage_, width_, isCalculated_);
    }

	std::vector<photoncpp::BitSet> getIslandRows() {
        return islandRows_;
    }

    int getIsLandsCount() {
        return isLandsCount_;
    }

	uint64_t getPixels() {
        return pixels_;
    }

    float getLayerPositionZ() {
        return layerPositionZ_;
    }

    void setLayerPositionZ(float layerPositionZ_) {
        layerPositionZ_ = layerPositionZ_;
    }

    float getLayerExposure() {
        return layerExposure_;
    }

    float getLayerOffTime() {
        return layerOffTimeSeconds_;
    }

    void setLayerExposure(float layerExposure_) {
        layerExposure_ = layerExposure_;
    }

    void setLayerOffTimeSeconds(float layerOffTimeSeconds_) {
        layerOffTimeSeconds_ = layerOffTimeSeconds_;
    }

    void unLink() {
        imageData_.clear();
        packedLayerImage_.clear();
        islandRows_.clear();
    }

    bool doExtendMargin() {
        return extendsMargin_;
    }

	private:
	bool checkMagin(std::vector<photoncpp::BitSet> & unpackedImage, uint32_t margin) {
        if (unpackedImage.size() > margin) {
            // check top margin rows
            for (uint32_t i = 0; i < margin; i++) {
                if (!unpackedImage.at(i).empty()) {
                    return true;
                }
            }
            // check bottom margin rows
            for (size_t i = unpackedImage.size() - margin; i < unpackedImage.size(); i++) {
                if (!unpackedImage.at(i).empty()) {
                    return true;
                }
            }

            for (size_t i = margin; i < unpackedImage.size() - margin; i++) {
				photoncpp::BitSet & row = unpackedImage.at(i);
				for (size_t j = 0; j < margin; j++)
				{
					if (row[j]) {
						return true;
					}
				}
				for (int32_t j = width_ - margin; j < width_; j++)
				{
					if (row[j]) {
						return true;
					}
				}
            }
        }
        return false;
    }

	public:
	PhotonLayer getLayer() {
        PhotonLayer photonLayer(width_, height_);
        photonLayer.unpackLayerImage(packedLayerImage_);
        return photonLayer;
    }

    void getUpdateLayer(PhotonLayer & photonLayer) {
        photonLayer.unpackLayerImage(packedLayerImage_);
    }

    void updateLayerIslands(PhotonLayer & photonLayer) {
        islandRows_.clear();
        isLandsCount_ = photonLayer.setIslands(islandRows_);
    }

    void saveLayer(PhotonLayer & photonLayer) {
        packedLayerImage_ = photonLayer.packLayerImage();
        imageData_ = photonLayer.packImageData();
        dataSize_ = imageData_.size();
        islandRows_.clear();
        isLandsCount_ = photonLayer.setIslands(islandRows_);
    }

	std::vector<photoncpp::BitSet> getUnknownRows() {
        return unpackImage(width_);
    }

    PhotonFileLayer & getAntiAlias(uint32_t a) {
        return antiAliasLayers_.at(a);
    }

	std::vector<PhotonFileLayer> & getAntiAlias() {
        return antiAliasLayers_;
    }
};
