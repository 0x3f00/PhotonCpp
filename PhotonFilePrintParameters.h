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


class PhotonFilePrintParameters {
public:
	float bottomLiftDistance_ = 5.0f;
    float bottomLiftSpeed_ = 300.0f;

    float liftingDistance_ = 5.0f;
    float liftingSpeed_ = 300.0f;
    float retractSpeed_ = 300.0f;

    float volumeMl_ = 0;
    float weightG_ =  0;
    float costDollars_ = 0;

    float bottomLightOffDelay_ = 0.0f;
    float lightOffDelay_ = 0.0f;
    int32_t bottomLayerCount_;

	int32_t p1_;
	int32_t p2_;
	int32_t p3_;
	int32_t p4_;


public:
	PhotonFilePrintParameters() = default;
	PhotonFilePrintParameters(int bottomLayerCount) {
        bottomLayerCount_ = bottomLayerCount;
    }

    PhotonFilePrintParameters(int parametersPos, const std::string & fileContent) {
        std::string data(fileContent.begin() + parametersPos, fileContent.begin() + parametersPos + getByteSize());
		std::stringstream ss(data);
        PhotonInputStream ds(ss);

        bottomLiftDistance_ = ds.readFloat();
        bottomLiftSpeed_ = ds.readFloat();

        liftingDistance_ = ds.readFloat();
        liftingSpeed_ = ds.readFloat();
        retractSpeed_ = ds.readFloat();

        volumeMl_ = ds.readFloat();
        weightG_ = ds.readFloat();
        costDollars_ = ds.readFloat();

        bottomLightOffDelay_ = ds.readFloat();
        lightOffDelay_ = ds.readFloat();
        bottomLayerCount_ = ds.readInt();

        p1_ = ds.readInt();
        p2_ = ds.readInt();
        p3_ = ds.readInt();
        p4_ = ds.readInt();
    }

    void save(PhotonOutputStream & os) {
        os.writeFloat(bottomLiftDistance_);
        os.writeFloat(bottomLiftSpeed_);

        os.writeFloat(liftingDistance_);
        os.writeFloat(liftingSpeed_);
        os.writeFloat(retractSpeed_);

        os.writeFloat(volumeMl_);
        os.writeFloat(weightG_);
        os.writeFloat(costDollars_);

        os.writeFloat(bottomLightOffDelay_);
        os.writeFloat(lightOffDelay_);
        os.writeInt(bottomLayerCount_);

        os.writeInt(p1_);
        os.writeInt(p2_);
        os.writeInt(p3_);
        os.writeInt(p4_);
    }

    int getByteSize() {
        return 4+4 +4+4+4 +4+4+4 +4+4+4 +4+4+4+4;
    }
};
