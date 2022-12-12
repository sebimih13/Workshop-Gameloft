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

	/** Getters / Setters */
	inline void setHeight(Vector3 _height) { height = _height; }

private:
	/** Generate model for object based on camera->getPosition() */
	Model* generateModel();

	/** Proprietati speciale */
	int nrCelule;
	int dimensiuneCelula;
	int offsetY;

	Vector3 height;
};

