#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

/** Forward Declarations */
class Shader;

class PostProcessor
{
public:
	/** Constructor */
	PostProcessor();

	/** Destructor */
	~PostProcessor();

	/** Load */
	void Load();

	/** Draw to Framebuffer */
	void BeginRender();
	void EndRender();

	/** Draw after post-processing */
	void Draw();

	/** Getters / Setters */
	inline void setGrayscale(bool enable) { grayscale = int(enable); }
	inline void setBlur(bool enable) { blur = int(enable); }
	inline void setSharpen(bool enable) { sharpen = int(enable); }

private:
	/** Render data */
	GLuint VBO, EBO;
	unsigned int nrIndices;

	GLuint FBO, RBO;
	GLuint texture;

	Shader* shader;

	/** Uniforms */
	int grayscale;
	GLuint grayscaleUniform;

	void setGrayscaleUniform();

	/*********************************************************/
	/******************** Kernel Effects *********************/
	/*********************************************************/

	GLuint offsetsUniform;

	void setOffsetsUniform();

	// TODO : initialize in constructor
	const float offset = 1.0f / 300.0f;
	const float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};

	/** Blur */
	int blur;
	GLuint blurUniform;
	GLuint blurKernelUniform;

	void setBlurUniform();
	void setBlurKernelUniform();

	const float blurKernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	/** Sharpen */
	int sharpen;
	GLuint sharpenUniform;
	GLuint sharpenKernelUniform;

	void setSharpenUniform();
	void setSharpenKernelUniform();

	const float sharpenKernel[9] = {
		 0.0f, -1.0f,  0.0f,
		-1.0f,  5.0f, -1.0f,
		 0.0f, -1.0f,  0.0f
	};
};

