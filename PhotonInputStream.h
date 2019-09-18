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
 *  by bn on 01/07/2018.
 */

class PhotonInputStream {
private:
	//DataInputStream dataInputStream;
    std::istream & inputStream_;
//    std::vector<char> byteBuffer;

public:
	PhotonInputStream(std::istream & inputStream) : inputStream_ (inputStream) {
    }

    int read() { // ??
		assert(0);
        return (int)inputStream_.tellg();
    }

    int read(std::vector<char> bytes, int offset, int readLen) {
		assert(0);
		return 0;// inputStream_.read(bytes, offset, readLen);
    }

    void readFully(std::vector<char> & bytes) {
		if(!bytes.empty())
			inputStream_.read(&bytes[0], bytes.size());
    }

    void readFully(std::vector<char> bytes, int offset, int readLen) {
		assert(0);
        //dataInputStream.readFully(bytes, offset, readLen);
    }

    int skipBytes(int n) {
		assert(0);
        inputStream_.ignore(n);
		return n;
    }

    bool readBoolean() {
		char res;
        inputStream_ >> res;
		return !!res;
    }

    char readByte() {
		char res;
		inputStream_ >> res;
		return res;
    }

    int readUnsignedByte() {
		char res;
		inputStream_.read(&res, 1);
		return (uint8_t)res;
    }

    short readShort() {
		std::vector<char> buf({0, 0});
        readFully(buf);
        return (short)((buf[1] & 0xff) << 8 | (buf[0] & 0xff));
    }

    int readUnsignedShort() {
		std::vector<char> buf({ 0, 0 });
		readFully(buf);
		return ((buf[1] & 0xff) << 8 | (buf[0] & 0xff));
    }

/*    char readChar() {
        dataInputStream.readFully(byteBuffer, 0, 2);
        return (char)((byteBuffer[1] & 0xff) << 8 | (byteBuffer[0] & 0xff));
    }*/

	uint32_t readInt() {
		std::vector<char> buf;
		buf.resize(4);
        readFully(buf);
        return (buf[3]) << 24 | (buf[2] & 0xff) << 16 |
                (buf[1] & 0xff) << 8 | (buf[0] & 0xff);
    }

    uint64_t readInt64() {
		std::vector<char> buf;
		buf.resize(8);
		readFully(buf);
		return (uint64_t)(buf[7]) << 56 | (uint64_t)(buf[6]&0xff) << 48 |
                (uint64_t)(buf[5] & 0xff) << 40 | (uint64_t)(buf[4] & 0xff) << 32 |
                (uint64_t)(buf[3] & 0xff) << 24 | (uint64_t)(buf[2] & 0xff) << 16 |
                (uint64_t)(buf[1] & 0xff) <<  8 | (uint64_t)(buf[0] & 0xff);
    }

    float readFloat() {
		float res;
		int32_t value = readInt();
		res = *reinterpret_cast<float*>(&value);
        return res;
    }

    double readDouble() {
		double res;
		int64_t value = readInt64();
		res = *reinterpret_cast<double*>(&value);
		return res;
    }

/*    @Deprecated
    @Override
    public final String readLine() throws IOException {
        return dataInputStream.readLine();
    }*/

    std::string readUTF() {
		std::string res;
		std::getline(inputStream_, res, '\0');
        return res;
    }

    int available() {
		assert(0);
		return 1;
//        return inputStream_.available();
    }

    void close() {
		// inputStream_.close();
    }

};
