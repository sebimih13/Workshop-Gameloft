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

	void setPosition();					// TODO : aici sau in alta parte
	void setUV();						// TODO : aici sau in alta parte

	void setMVP(Matrix* MVP);			// TODO : aici sau in alta parte
	void setColor(Vector3* color);		// TODO : aici sau in alta parte
	void setTexture(GLint index);		// TODO : aici sau in alta parte
	void setNrCelule(GLint nrCelule);	// TODO : aici sau in alta parte
	void setHeight(Vector3* height);	// TODO : aici sau in alta parte

private:
	/** CONST variables */
	const int MAX_TEXTURES;				// TODO : change in constructor for more textures

	/** RENDER DATA */
	ShaderResource* resource;
	GLuint programID;

	/** Attributes */					// TODO : aici sau in alta parte
	GLuint positionAttribute;
	GLuint uvAttribute;

	/** Uniforms */						// TODO : aici sau in alta parte
	GLuint mvpMatrixUniform;
	GLuint colorUniform;
	std::vector<GLuint> textureUniforms;
	GLuint nrCeluleUniform;
	GLuint heightUniform;
};

