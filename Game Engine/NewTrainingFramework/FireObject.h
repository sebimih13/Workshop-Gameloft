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
	void Update() override;

private:
	/** Proprietati speciale */

	/** Uniforms */
	GLuint heightUniform;

	/** Set uniforms functions */
	void setHeight();
};

