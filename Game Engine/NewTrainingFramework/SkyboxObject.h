#pragma once

#include "SceneObject.h"

class SkyboxObject : public SceneObject
{
public:
	/** Constructor */
	SkyboxObject();

	/** Destructor */
	~SkyboxObject();

	/** Load model + shader + textures */
	void Load() override;

	/** Draw Object */
	void Draw() override;

	/** Update Terrain */
	void Update() override;

private:

};

