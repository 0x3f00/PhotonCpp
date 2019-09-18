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
 * by bn on 06/07/2018.
 */
class PhotonOutputStream {
private:
	std::ostream & outputStream_;


public:
	PhotonOutputStream(std::ostream & outputStream) : outputStream_ (outputStream) {
    }

    void write(std::vector<char> b, int off, int len) {
		if (!b.empty())
			outputStream_.write(b.data(), len);
    }

	void write(std::string b, int off, int len) {
		if (!b.empty())
			outputStream_.write(b.data(), len);
	}

    void writeBoolean(bool v) {
		outputStream_.put(v);
    }

    void writeByte(int v) {
		outputStream_.put(v);
    }

/*    @Deprecated
    @Override
    public void writeBytes(String s) throws IOException {
        dataOutputStream.writeBytes(s);
    }*/

    void writeChar(int v) {
        writeShort(v);
    }

    void writeChars(std::string s) {
		assert(0);
/*        for (int i = 0; i < s.length(); i++) {
            writeChar(s.charAt(i));
        }*/
    }

    void writeDouble(double v) {
		int64_t value = *reinterpret_cast<int64_t*>(&v);
		writeInt64(value);
    }

    void writeFloat(float v) {
		int32_t value = *reinterpret_cast<int32_t*>(&v);
		writeInt(value);
    }

    void write(int b) {
        writeInt(b);
    }

    void writeInt(int v) {
		outputStream_.put(0xFF & v);
		outputStream_.put(0xFF & (v >> 8));
		outputStream_.put(0xFF & (v >> 16));
		outputStream_.put(0xFF & (v >> 24));
    }

    void writeInt64(uint64_t v) {
		std::vector<char> bytes;
		std::reverse(bytes.begin(), bytes.end());
        outputStream_.write(bytes.data(), bytes.size());
    }

private:
	std::vector<char> longToBytes(uint64_t l) {
        std::vector<char> result(8, 0);
        for (int i = 7; i >= 0; i--) {
            result[i] = (char)(l & 0xFF);
            l >>= 8;
        }
        return result;
    }

public:
	void writeShort(int v) {
		outputStream_.put(0xFF & v);
		outputStream_.put(0xFF & (v >> 8));
    }

    void writeUTF(const std::string & str) {
		outputStream_.write(str.c_str(), str.length());
    }

    void close() {
        //outputStream.close();
    }


};
