#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

#include "Vertex.h"

class CollisionComponent
{
public:
	/** Constructor */
	CollisionComponent(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

	/** Destructor */
	~CollisionComponent();

	/** Calculate min/max vertex position values */
	void CalculateVertexPosition(Matrix& modelMatrix);
	
	/** Getters / Setters */
	inline GLuint getVBO() { return VBO; }
	inline GLuint getWiredEBO() { return wiredEBO; }

	inline GLuint getNrIndicesWired() { return nrIndicesWired; }

	inline std::vector<Vertex>& getVerticesData() { return verticesData; }

	/** Getters */
	inline float getMinX() { return worldMinX; }
	inline float getMaxX() { return worldMaxX; }

	inline float getMinY() { return worldMinY; }
	inline float getMaxY() { return worldMaxY; }

	inline float getMinZ() { return worldMinZ; }
	inline float getMaxZ() { return worldMaxZ; }

private:
	/** Render data */
	GLuint VBO;
	GLuint wiredEBO;

	GLuint nrIndicesWired;

	std::vector<Vertex> verticesData;

 	/** min/max values */
	float worldMinX, worldMaxX;
	float worldMinY, worldMaxY;
	float worldMinZ, worldMaxZ;
};

