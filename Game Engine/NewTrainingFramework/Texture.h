#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

enum TextureType
{
	DEFAULT_TEXTURETYPE,	// None

	TEXTURE_2D,				// 2d
	TEXTURE_CUBE

	// TODO : ADD MORE
};

enum TextureFilter
{
	DEFAULT_TEXTUREFILTER,	// None

	LINEAR,
	NEAREST
};

enum TextureWrapMode
{
	DEFAULT_TEXTUREWRAPMODE, // None

	CLAMP_TO_EDGE,
	REPEAT,
	MIRRORED_REPEAT
};

struct TextureResource
{
	int id;					// TODO : do i need this?
	std::string filePath;
	std::string fileName;

	TextureType type;
	TextureFilter minFilter, magFilter;
	TextureWrapMode wrapS, wrapT;
};

class Texture
{
public:
	/** Constructor */
	Texture(TextureResource* textureResource);

	/** Destructor */
	~Texture();

	/** Load */
	void Load();

	/** Getters / Setters */
	inline GLuint getTextureID() { return textureID; }
	inline GLuint getTextureType() { return textureType; }

private:
	TextureResource* resource;
	GLuint textureID;

	GLuint textureType;
	GLint format;

	/** Utilities Functions */
	void LoadCubeTexture(char* pDest, char* pSource, int textureWidth, int textureHeight, int textureBpp, int squareX, int squareY);
};

