#include "stdafx.h"
#include "PointLight.h"

#include "Shader.h"

PointLight::PointLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, Vector3 pos, float constant, float linear, float quadratic)
	:	Light(type, ambientColor, diffuseColor, specularColor), 
		position(pos), constant(constant), linear(linear), quadratic(quadratic)
{
	positionUniform = -1;
	constantUniform = -1;
	linearUniform = -1;
	quadraticUniform = -1;
}

void PointLight::LoadUniforms(Shader* shader, int lightIndex)
{
	// ambient + diffuse + specular - colors
	const std::string ambientName = "pointLights[" + std::to_string(lightIndex) + "].ambient";
	const std::string diffuseName = "pointLights[" + std::to_string(lightIndex) + "].diffuse";
	const std::string specularName = "pointLights[" + std::to_string(lightIndex) + "].specular";

	ambientUniform = glGetUniformLocation(shader->getProgramID(), ambientName.c_str());
	diffuseUniform = glGetUniformLocation(shader->getProgramID(), diffuseName.c_str());
	specularUniform = glGetUniformLocation(shader->getProgramID(), specularName.c_str());

	// position
	const std::string positionName = "pointLights[" + std::to_string(lightIndex) + "].position";
	positionUniform = glGetUniformLocation(shader->getProgramID(), positionName.c_str());

	// constant + linear + quadratic
	const std::string constantName = "pointLights[" + std::to_string(lightIndex) + "].constant";
	const std::string linearName = "pointLights[" + std::to_string(lightIndex) + "].linear";
	const std::string quadraticName = "pointLights[" + std::to_string(lightIndex) + "].quadratic";

	constantUniform = glGetUniformLocation(shader->getProgramID(), constantName.c_str());
	linearUniform = glGetUniformLocation(shader->getProgramID(), linearName.c_str());
	quadraticUniform = glGetUniformLocation(shader->getProgramID(), quadraticName.c_str());
}

void PointLight::SetUniforms()
{
	// Call parent method
	Light::SetUniforms();

	// Set other uniforms
	if (positionUniform != -1)
	{
		glUniform3fv(positionUniform, 1, &position.x);
	}

	if (constantUniform != -1)
	{
		glUniform1f(constantUniform, constant);
	}

	if (linearUniform != -1)
	{
		glUniform1f(linearUniform, linear);
	}

	if (quadraticUniform != -1)
	{
		glUniform1f(quadraticUniform, quadratic);
	}
}

