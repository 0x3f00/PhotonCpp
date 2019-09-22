// PhotonCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"


PhotonLayer png2rle(const std::string & bufferPng)
{
	char* img = nullptr;
	int w = 0;
	int h = 0;
	int n = 0;
	img = (char*)stbi_load_from_memory((unsigned char*)bufferPng.data(), bufferPng.length(), &w, &h, &n, 4);

	PhotonLayer res(w, h);
	for (int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
		{
			int pos = ((w * j) + i) * 4;
			if (img[pos])
				res.supported(i, j);
		}

	stbi_image_free(img);
	return res;
}

std::string get_model()
{
	const unsigned char gz[] = { 
120, 94, 147, 100, 248, 43, 196, 196, 192, 192, 80, 45, 210, 225, 212, 250,
250, 163, 19, 3, 131, 152, 51, 3, 18, 56, 123, 198, 199, 150, 129, 97,
129, 35, 3, 3, 11, 80, 220, 193, 129, 3, 40, 183, 128, 21, 72, 112,
49, 48, 20, 0, 169, 43, 64, 205, 204, 64, 186, 133, 145, 129, 225, 25,
144, 1, 164, 24, 102, 0, 197, 108, 128, 98, 32, 246, 127, 134, 255, 200,
198, 49, 232, 2, 121, 32, 60, 1, 72, 127, 97, 192, 4, 58, 201, 219,
140, 252, 163, 11, 189, 57, 13, 78, 71, 233, 36, 43, 24, 108, 81, 230,
2, 179, 228, 13, 206, 121, 109, 81, 230, 52, 184, 32, 5, 18, 13, 244,
6, 177, 59, 4, 33, 42, 64, 162, 32, 182, 188, 1, 68, 252, 130, 20,
50, 175, 67, 16, 194, 131, 232, 238, 16, 228, 50, 210, 73, 150, 67, 82,
9, 225, 131, 236, 1, 169, 4, 241, 100, 145, 100, 33, 234, 97, 34, 190,
82, 32, 53, 93, 94, 58, 201, 178, 96, 151, 173, 16, 100, 50, 0, 241,
100, 12, 252, 163, 13, 193, 174, 131, 137, 200, 26, 156, 137, 234, 177, 210,
145, 226, 48, 56, 37, 8, 211, 35, 111, 208, 227, 165, 35, 197, 9, 20,
1, 233, 81, 52, 120, 19, 197, 105, 160, 147, 28, 110, 36, 15, 12, 6,
16, 94, 2, 12, 48, 236, 97, 194, 98, 136, 8, 19, 33, 120, 152, 8,
34, 133, 137, 144, 1, 34, 76, 64, 42, 96, 97, 34, 136, 18, 38, 130,
112, 85, 160, 48, 17, 52, 64, 14, 19, 1, 180, 48, 17, 48, 64, 14,
19, 126, 140, 48, 129, 137, 32, 194, 132, 31, 37, 76, 248, 48, 194, 132,
31, 75, 152, 8, 162, 132, 137, 48, 52, 76, 14, 26, 48, 48, 60, 112,
96, 96, 104, 2, 166, 199, 5, 80, 90, 204, 217, 108, 71, 190, 89, 203,
141, 102, 179, 26, 197, 103, 198, 12, 12, 14, 14, 32, 204, 193, 128, 11,
64, 210, 171, 3, 48, 77, 154, 21, 99, 170, 129, 164, 107, 136, 154, 178,
50, 6, 6, 236, 106, 206, 216, 194, 210, 253, 154, 151, 216, 213, 212, 142,
130, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129,
209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209,
16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16,
24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24,
13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13,
129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129,
209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209,
16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 129, 209, 16,
24, 13, 129, 209, 16, 24, 13, 129, 209, 16, 24, 13, 1, 66, 33, 144,
209, 140, 164, 194, 174, 121, 52, 192, 70, 67, 96, 52, 4, 70, 67, 96,
52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52,
4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4,
70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70,
67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67,
96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96,
52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52,
4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4,
70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70, 67, 96, 52, 4, 70,
67, 96, 52, 4, 8, 134, 64, 250, 104, 24, 141, 134, 192, 104, 8, 140,
134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134,
192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192,
104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104,
8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8,
140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140,
134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134,
192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192,
104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104,
8, 140, 134, 192, 104, 8, 16, 12, 129, 204, 38, 36, 37, 246, 77, 163,
33, 54, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33,
48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48,
26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26,
2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2,
163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163,
33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33,
48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48,
26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26,
2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 64, 48, 4,
70, 111, 205, 28, 77, 36, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2,
163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163,
33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33,
48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48,
26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26,
2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2,
163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163,
33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33,
48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 48, 26, 2, 163, 33, 64,
56, 4, 178, 26, 145, 212, 56, 52, 142, 6, 217, 104, 8, 140, 134, 192,
104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104,
8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8,
140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140,
134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134,
192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192,
104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104,
8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8,
140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140, 134, 192, 104, 8, 140,
134, 192, 104, 8, 140, 134, 0, 193, 16, 72, 7, 0, 143, 56, 126, 118
	};

	std::string res;
	res.resize(100*1024);

	int len = stbi_zlib_decode_buffer(&res[0], res.length(), (const char*)gz, sizeof gz);

	res.resize(len);
	return res;
}

int main()
{
	get_model();

//	std::ifstream t("E:\\projects\\PhotonCpp\\_model.photon", std::ios_base::binary);
//	t.seekg(0, std::ios::end);
	std::streamoff size = 0;
//	std::streamoff size = t.tellg();
//	std::string buffer((unsigned int)size, ' ');
	std::string buffer = get_model();
	//	t.seekg(0);
//	t.read(&buffer[0], size);
//	t.close();

//	char * p = &buffer[0] + size;

	PhotonFileHeader photonFileHeader(buffer);
	PhotonFilePreview previewOne(photonFileHeader.getPreviewOneOffsetAddress(), buffer);
	PhotonFilePreview previewTwo(photonFileHeader.getPreviewTwoOffsetAddress(), buffer);
	
	PhotonFilePrintParameters photonFilePrintParameters;
	if (photonFileHeader.getVersion() > 1) 
	{
		PhotonFilePrintParameters dummy(photonFileHeader.getPrintParametersOffsetAddress(), buffer);
		photonFilePrintParameters = std::move(dummy);
	}

	int margin = 0;
	
	std::vector<PhotonFileLayer> layers = PhotonFileLayer::readLayers(photonFileHeader, buffer, margin);

//	PhotonFileLayer::calculateLayers(photonFileHeader,layers, 0);

	// load png
	std::ifstream ifPng("E:\\projects\\PhotonCpp\\fff-B_Cu-1.svg.png", std::ios_base::binary);
//	std::ifstream ifPng("E:\\projects\\PhotonCpp\\solid.png", std::ios_base::binary);
	
	ifPng.seekg(0, std::ios::end);
	size = ifPng.tellg();
	std::string bufferPng((unsigned int)size, ' ');
	ifPng.seekg(0);
	ifPng.read(&bufferPng[0], size);
	ifPng.close();

	PhotonLayer layerPng = png2rle(bufferPng);
	//std::vector<PhotonFileLayer> layersPng = PhotonFileLayer::cloneLayers(photonFileHeader, dataRle);
	int antiAliasLevel = 1;
	if (photonFileHeader.getVersion() > 1) {
		antiAliasLevel = photonFileHeader.getAntiAliasingLevel();
	}

	for (int i = 0; i < photonFileHeader.getNumberOfLayers(); i++) {
		PhotonFileLayer & layer = layers.at(i);
		layer.saveLayer(layerPng);
		if (antiAliasLevel > 1) {
			for (int a = 0; a < (antiAliasLevel - 1); a++) {
				layer.getAntiAlias(a).saveLayer(layerPng);
			}
		}
	}




	// writing is here:

	std::ofstream ofs("E:\\projects\\PhotonCpp\\_out.photon", std::ios_base::binary);

	int headerPos = 0;
	int previewOnePos = headerPos + photonFileHeader.getByteSize();
	int previewTwoPos = previewOnePos + previewOne.getByteSize();
	int layerDefinitionPos = previewTwoPos + previewTwo.getByteSize();

	int parametersPos = 0;
	if (photonFileHeader.getVersion() > 1) {
		parametersPos = layerDefinitionPos;
		layerDefinitionPos = parametersPos + photonFilePrintParameters.getByteSize();
	}

	int dataPosition = layerDefinitionPos + (PhotonFileLayer::getByteSize() * photonFileHeader.getNumberOfLayers() * antiAliasLevel);


	PhotonOutputStream os(ofs);

	photonFileHeader.save(os, previewOnePos, previewTwoPos, layerDefinitionPos, parametersPos);
	previewOne.save(os, previewOnePos);
	previewTwo.save(os, previewTwoPos);

	if (photonFileHeader.getVersion() > 1) {
		photonFilePrintParameters.save(os);
	}

	// Optimize order for speed read on photon
	for (int i = 0; i < photonFileHeader.getNumberOfLayers(); i++) {
		PhotonFileLayer & layer = layers.at(i);
		dataPosition = layer.savePos(dataPosition);
		if (antiAliasLevel > 1) {
			for (int a = 0; a < (antiAliasLevel - 1); a++) {
				dataPosition = layer.getAntiAlias(a).savePos(dataPosition);
			}
		}
	}

	// Order for backward compatibility with photon/cbddlp version 1
	for (int i = 0; i < photonFileHeader.getNumberOfLayers(); i++) {
		layers.at(i).save(os);
	}

	if (antiAliasLevel > 1) {
		for (int a = 0; a < (antiAliasLevel - 1); a++) {
			for (int i = 0; i < photonFileHeader.getNumberOfLayers(); i++) {
				layers.at(i).getAntiAlias(a).save(os);
			}
		}
	}

	// Optimize order for speed read on photon
	for (int i = 0; i < photonFileHeader.getNumberOfLayers(); i++) {
		PhotonFileLayer & layer = layers.at(i);
		layer.saveData(os);
		if (antiAliasLevel > 1) {
			for (int a = 0; a < (antiAliasLevel - 1); a++) {
				layer.getAntiAlias(a).saveData(os);
			}
		}
	}

    std::cout << "Hello World!\n"; 

	return 0;
}
