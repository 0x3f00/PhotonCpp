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
 *  by bn on 30/06/2018.
 */
class PhotonFileHeader {
private:
	int32_t header1_;
    int32_t version_;
    float bedXmm_;
    float bedYmm_;
    float bedZmm_;
    int32_t unknown1_;
    int32_t unknown2_;
    int32_t unknown3_;
    float layerHeightMilimeter_;
    float exposureTimeSeconds_;
    float exposureBottomTimeSeconds_;
    float offTimeSeconds_;
    int32_t bottomLayers_;
    uint32_t resolutionX_;
    uint32_t resolutionY_;

    int32_t previewOneOffsetAddress_;
    int32_t layersDefinitionOffsetAddress_;
    int32_t numberOfLayers_;

    int32_t previewTwoOffsetAddress_;

    int32_t printTimeSeconds_;
    PhotonProjectType projectType_;

    int32_t printParametersOffsetAddress_;
    int32_t printParametersSize_;
    int32_t antiAliasingLevel_;

    short lightPWM_;
    short bottomLightPWM_;

    int32_t unknown4_;
    int32_t unknown5_;
    int32_t unknown6_;

public:
	PhotonFileHeader(const std::string & fileContent) {
		std::stringstream ss(fileContent);
        PhotonInputStream ds(ss);

        header1_ = ds.readInt();
        version_ = ds.readInt();

        bedXmm_ = ds.readFloat();
        bedYmm_ = ds.readFloat();
        bedZmm_ = ds.readFloat();

        unknown1_ = ds.readInt();
        unknown2_ = ds.readInt();
        unknown3_ = ds.readInt();

        layerHeightMilimeter_ = ds.readFloat();
        exposureTimeSeconds_ = ds.readFloat();
        exposureBottomTimeSeconds_ = ds.readFloat();

        offTimeSeconds_ = ds.readFloat();
        bottomLayers_ = ds.readInt();

        resolutionX_ = ds.readInt();
        resolutionY_ = ds.readInt();

        previewOneOffsetAddress_ = ds.readInt();
        layersDefinitionOffsetAddress_ = ds.readInt();

        numberOfLayers_ = ds.readInt();

        previewTwoOffsetAddress_ = ds.readInt();
        printTimeSeconds_ = ds.readInt();

        projectType_ = (ds.readInt() ? PhotonProjectType::lcdMirror : PhotonProjectType::cast);

        printParametersOffsetAddress_ = ds.readInt();
        printParametersSize_ = ds.readInt();
        antiAliasingLevel_ = ds.readInt();

        lightPWM_ = ds.readShort();
        bottomLightPWM_ = ds.readShort();

        unknown4_ = ds.readInt();
        unknown5_ = ds.readInt();
        if (version_>1) {
            unknown6_ = ds.readInt();
        }
    }

    int getByteSize() {
        return 4+4 + 4+4+4 + 4+4+4 + 4+4+4 + 4+4 + 4+4 + 4+4 + 4 + 4+4 + 4 + 4+4+4 +2+2 +4+4+ (version_>1?4:0);
    }

    void save(PhotonOutputStream os, int previewOnePos, int previewTwoPos, int layerDefinitionPos, int parametersPos) {
        previewOneOffsetAddress_ = previewOnePos;
        previewTwoOffsetAddress_ = previewTwoPos;
        layersDefinitionOffsetAddress_ = layerDefinitionPos;
        printParametersOffsetAddress_ = parametersPos;

        os.writeInt(header1_);
        os.writeInt(version_);

        os.writeFloat(bedXmm_);
        os.writeFloat(bedYmm_);
        os.writeFloat(bedZmm_);

        os.writeInt(unknown1_);
        os.writeInt(unknown2_);
        os.writeInt(unknown3_);

        os.writeFloat(layerHeightMilimeter_);
        os.writeFloat(exposureTimeSeconds_);
        os.writeFloat(exposureBottomTimeSeconds_);

        os.writeFloat(offTimeSeconds_);
        os.writeInt(bottomLayers_);

        os.writeInt(resolutionX_);
        os.writeInt(resolutionY_);

        os.writeInt(previewOneOffsetAddress_);
        os.writeInt(layersDefinitionOffsetAddress_);

        os.writeInt(numberOfLayers_);

        os.writeInt(previewTwoOffsetAddress_);
        os.writeInt(printTimeSeconds_);

        os.writeInt(projectType_);

        os.writeInt(printParametersOffsetAddress_);
        os.writeInt(printParametersSize_);
        os.writeInt(antiAliasingLevel_);

        os.writeShort(lightPWM_);
        os.writeShort(bottomLightPWM_);

        os.writeInt(unknown4_);
        os.writeInt(unknown5_);
        if (version_>1) {
            os.writeInt(unknown6_);
        }
    }


    int getPreviewOneOffsetAddress() const {
        return previewOneOffsetAddress_;
    }

    int getPreviewTwoOffsetAddress() const {
        return previewTwoOffsetAddress_;
    }

    int getNumberOfLayers() const {
        return numberOfLayers_;
    }

    int getLayersDefinitionOffsetAddress() const {
        return layersDefinitionOffsetAddress_;
    }

    float getNormalExposure() const {
        return exposureTimeSeconds_;
    }

    float getOffTime() const {
        return offTimeSeconds_;
    }

	uint32_t getResolutionX() const {
        return resolutionX_;
    }

	uint32_t getResolutionY() const {
        return resolutionY_;
    }

    float getBuildAreaX() const {
        return bedXmm_;
    }

    float getBuildAreaY() const {
        return bedYmm_;
    }

    float getLayerHeight() const {
        return layerHeightMilimeter_;
    }

    int getBottomLayers() const {
        return bottomLayers_;
    }

    float getBottomExposureTimeSeconds() const {
        return exposureBottomTimeSeconds_;
    }

    float getOffTimeSeconds() const {
        return offTimeSeconds_;
    }

    float getExposureTimeSeconds() const {
        return exposureTimeSeconds_;
    }

    void unLink() {
    }

    void setExposureTimeSeconds(float exposureTimeSeconds) {
        exposureTimeSeconds_ = exposureTimeSeconds;
    }

    void setExposureBottomTimeSeconds(float exposureBottomTimeSeconds) {
        exposureBottomTimeSeconds_ = exposureBottomTimeSeconds;
    }

    void setOffTimeSeconds(float offTimeSeconds) {
        offTimeSeconds_ = offTimeSeconds;
    }

    void setBottomLayers(int bottomLayers) {
        bottomLayers_ = bottomLayers;
    }

    int getVersion() const {
        return version_;
    }

    int getPrintParametersOffsetAddress() const {
        return printParametersOffsetAddress_;
    }

    int getPrintParametersSize() const {
        return printParametersSize_;
    }

    int getAntiAliasingLevel() const {
        return antiAliasingLevel_;
    }

    void setAntiAliasingLevel(int antiAliasingLevel) {
        antiAliasingLevel_ = antiAliasingLevel;
    }

    void makeVersion(int i) {
        version_ = i;
        antiAliasingLevel_ = 1;
        lightPWM_ = 255;
        bottomLightPWM_ = 255;
    }
};
