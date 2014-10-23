//
//  RGBAImage.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/17/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "RGBAImage.h"
#include "EngineUtil.h"

RGBAImage::~RGBAImage()
{
	if (textureId != NULL_HANDLE) glDeleteTextures(1, &textureId);
	if (samplerId != NULL_HANDLE) glDeleteSamplers(1, &samplerId);
}

bool RGBAImage::loadPNG(const string &fileName, bool doFlipY)
{
	string fullName;
	getFullFileName(fileName, fullName);
	unsigned error = lodepng::decode(pixels, width, height, fullName.c_str());
	if (error) {
		ERROR(lodepng_error_text(error), false);
		return false;
	}
    
	if (doFlipY) flipY(); // PNGs go top-to-bottom, OpenGL is bottom-to-top
	name = fileName;
	return true;
}

bool RGBAImage::writeToPNG(const string &fileName)
{
	unsigned error = lodepng::encode(fileName.c_str(), pixels, width, height);
	if (error) {
		ERROR(lodepng_error_text(error), false);
		return false;
	}
	return true;
}

void RGBAImage::flipY(void)
{
	unsigned int *a, *b;
	unsigned int temp;
    
	for (int y = 0; y < (int)height / 2; y++)
	{
		a = &pixel(0, y);
		b = &pixel(0, height - 1 - y);
		for (int x = 0; x < (int)width; x++) {
			temp = a[x];
			a[x] = b[x];
			b[x] = temp;
		}
	}
}

void RGBAImage::sendToOpenGL(GLuint magFilter, GLuint minFilter, bool createMipMap)
{
	if (width <= 0 || height <= 0) return;
    
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	if (createMipMap) glGenerateMipmap(GL_TEXTURE_2D);
    
	glGenSamplers(1, &samplerId);
	glBindSampler(textureId, samplerId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
}