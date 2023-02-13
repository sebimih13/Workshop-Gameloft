#include "stdafx.h"
#include "DirectionalLight.h"

#include "Shader.h"

DirectionalLight::DirectionalLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, Vector3 dir)
	: Light(type, ambientColor, diffuseColor, specularColor), direction(dir)
{
	directionUniform = -1;
}

void DirectionalLight::LoadUniforms(Shader* shader, int lightIndex)
{
	// ambient + diffuse + specular - colors
	const std::string ambientName = "directionalLights[" + std::to_string(lightIndex) + "].ambient";
	const std::string diffuseName = "directionalLights[" + std::to_string(lightIndex) + "].diffuse";
	const std::string specularName = "directionalLights[" + std::to_string(lightIndex) + "].specular";

	ambientUniform = glGetUniformLocation(shader->getProgramID(), ambientName.c_str());
	diffuseUniform = glGetUniformLocation(shader->getProgramID(), diffuseName.c_str());
	specularUniform = glGetUniformLocation(shader->getProgramID(), specularName.c_str());

	// direction
	const std::string directionName = "directionalLights[" + std::to_string(lightIndex) + "].direction";
	directionUniform = glGetUniformLocation(shader->getProgramID(), directionName.c_str());
}

void DirectionalLight::SetUniforms()
{
	// Call parent method
	Light::SetUniforms();

	// Set other uniforms
	if (directionUniform != -1)
	{
		glUniform3fv(directionUniform, 1, &direction.x);
	}
}

