#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

struct ShaderResource
{
	int id;					// TODO : do i need this?
	std::string filePath; 

	std::string fileVS;
	std::string fileFS;
};

class Shader
{
public:
	/** Constructor */
	Shader(ShaderResource* shaderResource);

	/** Destructor */
	~Shader();

	/** Load */
	void Load();

	/** Getters / Setters */
	inline GLuint getProgramID() { return programID; }

	void setPosition();			// TODO : aici sau in alta parte
	void setUV();				// TODO : aici sau in alta parte

	void setMVP(Matrix* MVP);	// TODO : aici sau in alta parte

private:
	ShaderResource* resource;
	GLuint programID;

	/** Attributes */			// TODO : aici sau in alta parte
	GLuint positionAttribute;
	GLuint uvAttribute;

	/** Uniforms */				// TODO : aici sau in alta parte
	GLuint mvpMatrixUniform;
};

