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

	int textureWidth, textureHeight, texureBpp;
	char* textureData = LoadTGA(file.c_str(), &textureWidth, &textureHeight, &texureBpp);

	// TODO : DEBUG
	std::cout << "\nTexture file : " << file << '\n';
	std::cout << "textureWidth : " << textureWidth << "\n";
	std::cout << "textureHeight : " << textureHeight << "\n";
	std::cout << "texureBpp : " << texureBpp << "\n";

	switch (texureBpp)
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

				0 = dreapta	= GL_TEXTURE_CUBE_MAP_POSITIVE_X
				1 = stanga	= GL_TEXTURE_CUBE_MAP_NEGATIVE_X
				2 = sus		= GL_TEXTURE_CUBE_MAP_POSITIVE_Y
				3 = jos		= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
				4 = fata	= GL_TEXTURE_CUBE_MAP_POSITIVE_Z
				5 = spate	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z

			*/

			// TODO : DEBUG pt shader
			GLubyte cubePixels[6][3] =
			{
				// Face 0 - Red
				255, 0, 0,
				// Face 1 - Green,
				0, 255, 0,
				// Face 2 - Blue
				0, 0, 255,
				// Face 3 - Yellow
				255, 255, 0,
				// Face 4 - Purple
				255, 0, 255,
				// Face 5 - White
				255, 255, 255
			};

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);	// 0 = dreapta
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]); // 1 = stanga
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[2]);	// 2 = sus
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[3]);	// 3 = jos
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[4]);	// 4 = fata
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[5]);	// 5 = spate

			// TODO : pt debug
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) 
			{
				std::cout << "OpenGL error: " << err << '\n';	// => 1282 = GL_INVALID_OPERATION
			}

			int subWidth = textureWidth / 4;
			int subHeight = textureHeight / 3;

			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 * subWidth, 1 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 0
			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 2 * subWidth, 1 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 1

			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 1 * subWidth, 2 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 2
			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 1 * subWidth, 0 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 3

			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 1 * subWidth, 1 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 4
			//glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 3 * subWidth, 1 * subHeight, subWidth, subHeight, format, GL_UNSIGNED_BYTE, textureData);	// 5
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

