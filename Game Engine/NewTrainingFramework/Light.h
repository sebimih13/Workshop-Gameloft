#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

/** Forward Declarations */	
class Shader;

enum LightType
{
	LightType_DEFAULT,

	Point,
	Directional,
	Spotlight

	// TODO : ADD MORE
};

class Light
{
public:
	/** Constructor */
	Light(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor);

	/** Load */
	virtual void LoadUniforms(Shader* shader, int lightIndex) = 0;

	/** Set Uniforms */
	virtual void SetUniforms();

	/** Getters / Setters */
	inline LightType getType() { return type; }

protected:
	/** Properties */
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

	/** Uniforms */
	GLuint ambientUniform;
	GLuint diffuseUniform;
	GLuint specularUniform;

private:
	LightType type;
};

