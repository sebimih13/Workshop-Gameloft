#include "stdafx.h"
#include "Texture.h"

Texture::Texture(TextureResource* textureResource) : resource(textureResource)
{
	glGenTextures(1, &textureID);

	textureType = TextureType::TEXTURE_2D;
	format = GL_RGBA;

	switch (resource->type)
	{
		case TextureType::TEXTURE_2D:	textureType = GL_TEXTURE_2D;		break;
		case TextureType::TEXTURE_CUBE: textureType = GL_TEXTURE_CUBE_MAP;	break;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::Load()
{
	// Load .tga file
	std::string file = resource->filePath + resource->fileName;

	int textureWidth, textureHeight, textureBpp;
	char* textureData = LoadTGA(file.c_str(), &textureWidth, &textureHeight, &textureBpp);

	// TODO : DEBUG
	std::cout << "\nTexture file : " << file << '\n';
	std::cout << "textureWidth : " << textureWidth << "\n";
	std::cout << "textureHeight : " << textureHeight << "\n";
	std::cout << "texureBpp : " << textureBpp << "\n";

	switch (textureBpp)
	{
		case 32: format = GL_RGBA; break;
		case 24: format = GL_RGB;  break;
	}

	glBindTexture(textureType, textureID);

	switch (resource->type)
	{
		case TextureType::TEXTURE_2D:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, textureData);
		}
		break;

		case TextureType::TEXTURE_CUBE:
		{
			/*	Format textura:
				- 2 - -
				1 4 0 5
				- 3 - - 
			*/

			std::vector<std::pair<int, int>> faces;
			faces.push_back({ 1, 2 });	// 0 = dreapta	= GL_TEXTURE_CUBE_MAP_POSITIVE_X
			faces.push_back({ 1, 0 });	// 1 = stanga	= GL_TEXTURE_CUBE_MAP_NEGATIVE_X
			faces.push_back({ 0, 1 });	// 2 = sus		= GL_TEXTURE_CUBE_MAP_POSITIVE_Y
			faces.push_back({ 2, 1 });	// 3 = jos		= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
			faces.push_back({ 1, 1 });	// 4 = fata		= GL_TEXTURE_CUBE_MAP_POSITIVE_Z
			faces.push_back({ 1, 3 });	// 5 = spate	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z

			for (int i = 0; i < 6; i++)
			{
				int subWidth = textureWidth / 4;		// = 128
				int subHeight = textureHeight / 3;		// = 128

				char* subBuffer = new char[subWidth * subHeight * textureBpp / 8];
				LoadCubeTexture(subBuffer, textureData, textureWidth, textureHeight, textureBpp, faces[i].first, faces[i].second);

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, subWidth, subHeight, 0, format, GL_UNSIGNED_BYTE, subBuffer);
			}

			// TODO : pt debug
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) 
			{
				std::cout << "OpenGL error: " << err << '\n';	// => 1282 = GL_INVALID_OPERATION
			}
		}
		break;
	}

	// Set parameters
	switch (resource->minFilter)
	{
		case TextureFilter::LINEAR:	 glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	break;
		case TextureFilter::NEAREST: glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	break;
	}

	switch (resource->magFilter)
	{
		case TextureFilter::LINEAR:	 glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	break;
		case TextureFilter::NEAREST: glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	break;
	}

	switch (resource->wrapS)
	{
		case TextureWrapMode::REPEAT:			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);			 break;
		case TextureWrapMode::CLAMP_TO_EDGE:	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	 break;
		case TextureWrapMode::MIRRORED_REPEAT:	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); break;
	}

	switch (resource->wrapT)
	{
		case TextureWrapMode::REPEAT:			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);			 break;
		case TextureWrapMode::CLAMP_TO_EDGE:	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	 break;
		case TextureWrapMode::MIRRORED_REPEAT:	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); break;
	}

	// Unbind
	glBindTexture(textureType, 0);
}

void Texture::LoadCubeTexture(char* pDest, char* pSource, int textureWidth, int textureHeight, int textureBpp, int squareX, int squareY)
{
	/*	Format textura:

			- 2 - -				(0,0) (0,1) (0,2) (0,3)
			1 4 0 5		=>		(1,0) (1,1) (1,2) (1,3)
			- 3 - -				(2,0) (2,1) (2,2) (2,3)

	*/

	int subWidth = textureWidth / 4;
	int subHeight = textureHeight / 3;

	// Square offsets
	int offsetHeight = squareX * subHeight;
	int offsetWidth = squareY * subWidth * textureBpp / 8;

	int rowSize = textureWidth * textureBpp / 8;

	for (int i = 0; i < subHeight; i++)
	{
		char* pSrcRow = pSource + (textureHeight - i - 1 - offsetHeight) * rowSize + offsetWidth;

		for (int j = 0; j < subWidth; j++)
		{
			*pDest++ = pSrcRow[0];
			*pDest++ = pSrcRow[1];
			*pDest++ = pSrcRow[2];
			pSrcRow += 3;
		}
	}
}

