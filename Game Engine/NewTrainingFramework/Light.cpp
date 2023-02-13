#include "stdafx.h"
#include "Light.h"

Light::Light(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor)
	: type(type), ambient(ambientColor), diffuse(diffuseColor), specular(specularColor)
{
	ambientUniform = -1;
	diffuseUniform = -1;
	specularUniform = -1;
}

void Light::SetUniforms()
{
	if (ambientUniform != -1)
	{
		glUniform3fv(ambientUniform, 1, &ambient.x);
	}

	if (diffuseUniform != -1)
	{
		glUniform3fv(diffuseUniform, 1, &diffuse.x);
	}

	if (specularUniform != -1)
	{
		glUniform3fv(specularUniform, 1, &specular.x);
	}
}

