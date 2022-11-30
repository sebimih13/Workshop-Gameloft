#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

enum TextureType
{
	TEXTURE_2D,
	TEXTURE_3D
};

enum TextureFilter
{
	LINEAR,
	NEAREST
};

enum TextureWrapMode
{
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

private:
	TextureResource* resource;
	GLuint textureID;
};

