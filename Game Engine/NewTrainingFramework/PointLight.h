#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	/** Constructor */
	PointLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor,
				Vector3 pos, float constant, float linear, float quadratic);

	/** Load */
	void LoadUniforms(Shader* shader, int lightIndex) override;

	/** Set Uniforms */
	void SetUniforms() override;

	/** Getters / Setters */
	// TODO

private:
	/** Properties */
	Vector3 position;

	float constant;
	float linear;
	float quadratic;

	/** Uniforms */
	GLuint positionUniform;

	GLuint constantUniform;
	GLuint linearUniform;
	GLuint quadraticUniform;
};

