#include "stdafx.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(FontResource* fontResource)
{
	resource = fontResource;
}

Font::~Font()
{

}

void Font::Load()
{
	// initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR: Could not init FreeType Library\n";
	}
	else
	{
		std::cout << "FreeType Library VALID\n";
	}

	FT_Face face;
	if (FT_New_Face(ft, resource->filePath.c_str(), 0, &face))
	{
		std::cout << "ERROR: Failed to load font\n";
	}

	// set size to load guphs as
	FT_Set_Pixel_Sizes(face, 0, resource->fontSize);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// precompile first 128 ASCII characters
	for (GLubyte c = 0; c < 128; c++)
	{
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE:: Failed to load Glyph\n";
			continue;
		}

		// generate texture
		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		// TODO : check GL_RED
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// store the character
		Character character = {
			texture,
			uiVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			uiVec2(face->glyph->bitmap_left , face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.insert({ c, character });
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// destroy FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

