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

	/** Any scene object */				// TODO : muta in SceneObject.h  ???
	void setPosition();					// TODO : aici sau in alta parte
	void setNormal();					// TODO : aici sau in alta parte
	void setUV();						// TODO : aici sau in alta parte

	void setMVP(Matrix* MVP);			// TODO : aici sau in alta parte
	void setModelMatrixUniform(Matrix* model);

	void setColor(Vector3* color);		// TODO : aici sau in alta parte
	void setTexture(GLint index);		// TODO : aici sau in alta parte

	/** Lights */
	void setLightsCount(GLint count);
	void setCameraViewPosition(Vector3* pos);

	void setLightPosition(std::vector<Vector3>& positions);

	/** Ambiental */
	void setAmbientalLightColor(Vector3* color);
	void setAmbientalLightStrength(GLfloat strength);

	/** Diffuse */
	void setDiffuseLightColor(std::vector<Vector3>& colors);
	void setDiffuseLightStrength(std::vector<GLfloat>& strengths);

	/** Specular */
	void setSpecularLightColor(std::vector<Vector3>& colors);
	void setSpecularLightStrength(std::vector<GLfloat>& strengths);

private:
	/** CONST variables */
	const int MAX_TEXTURES;				// TODO : change in constructor for more textures

	/** RENDER DATA */
	ShaderResource* resource;
	GLuint programID;

	/** Attributes */					// TODO : aici sau in alta parte
	GLuint positionAttribute;
	GLuint normalAttribute;
	GLuint uvAttribute;

	/** Uniforms */						// TODO : aici sau in alta parte
	GLuint mvpMatrixUniform;
	GLuint modelMatrixUniform;

	GLuint colorUniform;
	std::vector<GLuint> textureUniforms;

	GLuint lightPositionUniform;
	GLuint viewPositionUniform;

	GLuint lightsCountUniform;

	GLuint ambientalLightColorUniform;
	GLuint ambientalLightStrengthUniform;

	GLuint diffuseLightColorUniform;
	GLuint diffuseLightStrengthUniform;

	GLuint specularLightColorUniform;
	GLuint specularLightStrengthUniform;
};

