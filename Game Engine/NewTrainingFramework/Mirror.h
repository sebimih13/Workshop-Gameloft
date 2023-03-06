#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

/** Forward Declarations */
class Shader;


class Mirror
{
public:
	/** Constructor */
	Mirror();

	/** Destructor */
	~Mirror();

	/** Load */
	void Load();

	/** Draw to Framebuffer */
	void BeginRender();
	void EndRender();

	/** Draw */
	void Draw();

private:
	/** Render data */
	GLuint VBO, EBO;
	unsigned int nrIndices;

	GLuint FBO, RBO;
	GLuint texture;

	Shader* shader;
};

