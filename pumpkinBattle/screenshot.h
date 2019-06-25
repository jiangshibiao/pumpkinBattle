#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <windows.h>	
#include <stdio.h>

void WriteBitmapFile(const char * filename, int width, int height, unsigned char * bitmapData)
{
	//填充BITMAPFILEHEADER
	BITMAPFILEHEADER bitmapFileHeader;
	memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType = 0x4d42;	//BM
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BITMAPINFOHEADER
	BITMAPINFOHEADER bitmapInfoHeader;
	memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = 0;
	bitmapInfoHeader.biSizeImage = width * abs(height) * 3;

	//////////////////////////////////////////////////////////////////////////
	FILE * filePtr;			//连接要保存的bitmap文件用
	unsigned char tempRGB;	//临时色素
	int imageIdx;

	//交换R、B的像素位置,bitmap的文件放置的是BGR,内存的是RGB
	for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapData[imageIdx];
		bitmapData[imageIdx] = bitmapData[imageIdx + 2];
		bitmapData[imageIdx + 2] = tempRGB;
	}

	fopen_s(&filePtr, filename, "wb");

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, filePtr);

	fclose(filePtr);
}

void screeshot(int width, int height){
	unsigned char *screenData = new unsigned char[width * height * 3];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenData);
	WriteBitmapFile("screenshot.bmp", width, height, screenData);
	delete []screenData;
}

