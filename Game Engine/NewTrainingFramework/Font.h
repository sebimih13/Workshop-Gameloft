#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

struct FontResource
{
	int id;					// TODO : do i need this?
	unsigned int fontSize;
	std::string filePath;
};

struct uiVec2
{
	uiVec2(unsigned int x, unsigned int y) : x(x), y(y)
	{  }

	unsigned int x, y;
};

/** Holds all state information relevant to a character as loaded using FreeType */
struct Character
{
	unsigned int TextureID;	// ID handle of the glyph texture
	uiVec2 Size;			// size of glyph
	uiVec2 Bearing;			// offset from baseline to left/top of glyph
	unsigned int Advance;	// horizontal offset to advance to next glyph
};

class Font
{
public:
	/** Constructor */
	Font(FontResource* fontResource);

	/** Destructor */
	~Font();

	/** Load - pre-compile a list of characters from given font */
	void Load();

	/** Gettrs */
	// TODO : error
	// Character getCharacter(char c) { return characters[c]; }

private:
	/** Holds a list of precompiled Characters */
	std::map<char, Character> characters;

	FontResource* resource;
};

