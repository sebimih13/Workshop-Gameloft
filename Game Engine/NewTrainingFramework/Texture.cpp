#include "stdafx.h"
#include "Texture.h"

Texture::Texture(TextureResource* textureResource) : resource(textureResource)
{
	glGenTextures(1, &textureID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::Load()
{
	std::string file = resource->filePath + resource->fileName;

	int textureWidth, textureHeight, texureBpp;
	char* textureData = LoadTGA(file.c_str(), &textureWidth, &textureHeight, &texureBpp);

	std::cout << "Texture file : " << file << '\n';
	std::cout << "textureWidth : " << textureWidth << "\n";
	std::cout << "textureHeight : " << textureHeight << "\n";
	std::cout << "texureBpp : " << texureBpp << "\n";

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (resource->minFilter)
	{
		case TextureFilter::LINEAR:	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	break;
		case TextureFilter::NEAREST: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	break;
	}

	switch (resource->magFilter)
	{
		case TextureFilter::LINEAR:	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	break;
		case TextureFilter::NEAREST: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	break;
	}

	switch (resource->wrapS)
	{
	case TextureWrapMode::REPEAT:			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			break;
	case TextureWrapMode::CLAMP_TO_EDGE:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	break;
	case TextureWrapMode::MIRRORED_REPEAT:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	break;
	}

	switch (resource->wrapT)
	{
	case TextureWrapMode::REPEAT:			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			break;
	case TextureWrapMode::CLAMP_TO_EDGE:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	break;
	case TextureWrapMode::MIRRORED_REPEAT:	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	break;
	}

	// TODO : CHOOSE GL_RGB / GL_RGBA	based on texureBpp

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

