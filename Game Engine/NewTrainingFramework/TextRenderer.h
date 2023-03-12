#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

/** Forward Declarations */
class Shader;

class TextRenderer
{
public:
	/** Destructor */
	~TextRenderer();

	/** Get Instance */
	static TextRenderer* getInstance();

	/** Draw - render a string of text using the desired font */
	void Draw(std::string text, int fontID, float x, float y, float scale, Vector3 color);

private:
	/** Constructor */
	TextRenderer();

	/** Instance */
	static TextRenderer* instance;

	/** Render data */
	GLuint VBO;
};

