#pragma once

#include "Light.h"

class SpotLight : public Light
{
public:
	/** Constructor */
	SpotLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor,
				Vector3 pos, Vector3 dir,
				float constant, float linear, float quadratic,
				float cutOff, float outerCutOff);

	/** Load */
	void LoadUniforms(Shader* shader, int lightIndex) override;

	/** Set Uniforms */
	void SetUniforms() override;

	/** Getters / Setters */
	// TODO

private:
	/** Properties */
	Vector3 position;
	Vector3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	/** Uniforms */
	GLuint positionUniform;
	GLuint directionUniform;

	GLuint constantUniform;
	GLuint linearUniform;
	GLuint quadraticUniform;

	GLuint cutOffUniform;
	GLuint outerCutOffUniform;
};

