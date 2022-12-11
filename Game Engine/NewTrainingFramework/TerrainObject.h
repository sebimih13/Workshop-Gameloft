#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject
{
public:
	/** Constructor */
	TerrainObject();

	/** Destructor */
	~TerrainObject();

	/** Load model + shader + textures */
	void Load() override;

	/** Draw Object */
	void Draw() override;

private:
	// TODO : camera trb sa fie setata
	/** Generate model for object based on camera->getPosition() */
	Model* generateModel();

	/** Proprietati speciale */
	int nrCelule;					// TODO : nr par
	int dimensiuneCelula;
	int offsetY;
};

