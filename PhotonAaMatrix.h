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


class PhotonAaMatrix {

public:
	std::array<std::array<int, 5>, 5> aaMatrix_;

private:
	std::array<bool, 5> hasDivisor_;

public: 
	void clear() {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                aaMatrix_[y][x] = 0;
            }
        }
    }

    void set(int x, int y, int val) {
        aaMatrix_[y-1][x-1] = val;
    }

	photoncpp::MatrixInt calc(const photoncpp::MatrixInt & source) {
		photoncpp::MatrixInt target;

        if (!source.empty()) {
            target = source;

            int divisor = 0;
            for (int y = 0; y < 5; y++) {
                int rowDivistor = 0;
                for (int x = 0; x < 5; x++) {
                    rowDivistor += aaMatrix_[y][x];
                }
                hasDivisor_[y] = (rowDivistor>0);
                divisor += rowDivistor;
            }

            if (divisor >0) {
                int height = source.size();
                if (height > 0) {
                    int width = source[0].size();
                    if (width > 0) {
                        int sum;;
                        int dy;
                        int dx;
                        for (int y = 0; y < height; y++) {
                            for (int x = 0; x < width; x++) {
                                sum = 0;
                                for (int cy = -2; cy <= 2; cy++) {
                                    if (hasDivisor_[2+cy])
                                        for (int cx = -2; cx <= 2; cx++) {
                                            dy = y+cy;
                                            dx = x+cx;
                                            if (dy>=0 && dy< height) {
                                                if (dx>=0 && dx< width) {
                                                    sum += source[dy][dx] * aaMatrix_[2+cy][2+cx];
                                                } else {
                                                    sum += source[y][x] * aaMatrix_[2+cy][2+cx];
                                                }
                                            } else {
                                                sum += source[y][x] * aaMatrix_[2+cy][2+cx];
                                            }
                                        }
                                }
                                target[y][x] = sum / divisor;
                            }
                        }
                    }
                }
            }
        }
        return target;
    }
};


