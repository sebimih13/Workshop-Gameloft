#include "stdafx.h"
#include "SpotLight.h"

#include "Shader.h"

SpotLight::SpotLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, Vector3 pos, Vector3 dir, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
	:	Light(type, ambientColor, diffuseColor, specularColor),
		position(pos), direction(dir),
		constant(constant), linear(linear), quadratic(quadratic),
		cutOff(cutOff), outerCutOff(outerCutOff)
{
	positionUniform = -1;
	directionUniform = -1;
	constantUniform = -1;
	linearUniform = -1;
	quadraticUniform = -1;
	cutOffUniform = -1;
	outerCutOffUniform = -1;
}

void SpotLight::LoadUniforms(Shader* shader, int lightIndex)
{
	// ambient + diffuse + specular - colors
	const std::string ambientName = "spotlights[" + std::to_string(lightIndex) + "].ambient";
	const std::string diffuseName = "spotlights[" + std::to_string(lightIndex) + "].diffuse";
	const std::string specularName = "spotlights[" + std::to_string(lightIndex) + "].specular";

	ambientUniform = glGetUniformLocation(shader->getProgramID(), ambientName.c_str());
	diffuseUniform = glGetUniformLocation(shader->getProgramID(), diffuseName.c_str());
	specularUniform = glGetUniformLocation(shader->getProgramID(), specularName.c_str());

	// position + direction
	const std::string positionName = "spotlights[" + std::to_string(lightIndex) + "].position";
	const std::string directionName = "spotlights[" + std::to_string(lightIndex) + "].direction";

	positionUniform = glGetUniformLocation(shader->getProgramID(), positionName.c_str());
	directionUniform = glGetUniformLocation(shader->getProgramID(), directionName.c_str());
	
	// constant + linear + quadratic
	const std::string constantName = "spotlights[" + std::to_string(lightIndex) + "].constant";
	const std::string linearName = "spotlights[" + std::to_string(lightIndex) + "].linear";
	const std::string quadraticName = "spotlights[" + std::to_string(lightIndex) + "].quadratic";

	constantUniform = glGetUniformLocation(shader->getProgramID(), constantName.c_str());
	linearUniform = glGetUniformLocation(shader->getProgramID(), linearName.c_str());
	quadraticUniform = glGetUniformLocation(shader->getProgramID(), quadraticName.c_str());

	// cutOff + outerCutOff
	const std::string cutOffName = "spotlights[" + std::to_string(lightIndex) + "].cutOff";
	const std::string outerCutOffName = "spotlights[" + std::to_string(lightIndex) + "].outerCutOff";

	cutOffUniform = glGetUniformLocation(shader->getProgramID(), cutOffName.c_str());
	outerCutOffUniform = glGetUniformLocation(shader->getProgramID(), outerCutOffName.c_str());
}

void SpotLight::SetUniforms()
{
	// Call parent method
	Light::SetUniforms();

	// Set other uniforms
	if (positionUniform != -1)
	{
		glUniform3fv(positionUniform, 1, &position.x);
	}

	if (directionUniform != -1)
	{
		glUniform3fv(directionUniform, 1, &direction.x);
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

	if (cutOffUniform != -1)
	{
		glUniform1f(cutOffUniform, cutOff);
	}

	if (outerCutOffUniform != -1)
	{
		glUniform1f(outerCutOffUniform, outerCutOff);
	}
}

