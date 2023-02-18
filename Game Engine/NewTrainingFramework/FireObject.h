#pragma once

#include "SceneObject.h"

class FireObject : public SceneObject
{
public:
	/** Constructor */
	FireObject();

	/** Destructor */
	~FireObject();

	/** Load model + shader + textures */
	void Load() override;

	/** Draw Object */
	void Draw() override;

	/** Update Terrain */
	void Update(float deltaTime) override;

	/** Getters / Setters */
	inline void setDispMax(float value) { dispMax = value; }

private:
	/** Proprietati speciale */
	float time;
	float dispMax;

	/** Uniforms */
	GLuint timeUniform;
	GLuint dispMaxUniform;

	/** Set uniforms functions */
	void setTime();
	void setDispMax();
};

