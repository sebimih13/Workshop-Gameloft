#pragma once

#include "SceneObject.h"

class TerrainObject : public SceneObject
{
public:
	/** Constructor */
	TerrainObject();

	/** Destructor */
	~TerrainObject();

	void generateModel(Vector3 initialPosition);

	void Load() override;
	void Draw() override;

private:
	/** Proprietati */
	int nrCelule;			// TODO : nr par
	int dimensiuneCelula;
	int offsetY;

	/** Render data */
	GLuint VBO;
	GLuint EBO;
};

