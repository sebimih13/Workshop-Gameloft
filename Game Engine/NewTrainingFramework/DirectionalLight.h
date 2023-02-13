#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	/** Constructor */
	DirectionalLight(LightType type, Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, Vector3 dir);

	/** Load */
	void LoadUniforms(Shader* shader, int lightIndex) override;

	/** Set Uniforms */
	void SetUniforms() override;

	/** Getters / Setters */
	// TODO

private:
	/** Properties */
	Vector3 direction;

	/** Uniforms */
	GLuint directionUniform;
};

