


CC=g++
CPPFLAGS=-I.
DEPS = pch.h PhotonAaMatrix.h PhotonDot.h PhotonFileHeader.h \
       PhotonFileLayer.h PhotonFilePreview.h PhotonFilePrintParameters.h \
       PhotonInputStream.h PhotonLayer.h PhotonLine.h PhotonMatrix.h \
       PhotonOutputStream.h PhotonProjectType.h PhotonRow.h stb_image.h

PngToPhoton: 
	$(CC) -o PngToPhoton PhotonCpp.cpp $(CPPFLAGS)



