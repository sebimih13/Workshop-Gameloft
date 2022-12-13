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

	/** Update Terrain */
	void Update() override;

	/** Getters / Setters */
	inline void setHeight(Vector3 _height) { height = _height; }

private:
	/** Generate model for object based on camera->getPosition() */
	Model* generateModel();

	/** Proprietati speciale */
	int nrCelule;
	int dimensiuneCelula;
	int offsetX;
	int offsetY;
	int offsetZ;

	Vector3 height;
	Vector3 currentPosition;	// TODO : pt a evita asta generateModel() va genera cu punctul de coordonata(0, 0, 0) (acum e fata de unde e camera)
};

