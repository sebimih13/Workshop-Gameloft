#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

#include "Vertex.h"

class CollisionComponent
{
public:
	/** Constructor */
	CollisionComponent(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

	~CollisionComponent();

	/** Getters / Setters */
	inline GLuint getVBO() { return VBO; }
	inline GLuint getWiredEBO() { return wiredEBO; }

	inline GLuint getNrIndicesWired() { return nrIndicesWired; }

	inline std::vector<Vertex>& getVerticesData() { return verticesData; }

	/** Update min/max */
	inline void updateMinX(float newValue) { minX = newValue; }
	inline void updateMaxX(float newValue) { maxX = newValue; }

	inline void updateMinY(float newValue) { minY = newValue; }
	inline void updateMaxY(float newValue) { maxY = newValue; }

	inline void updateMinZ(float newValue) { minZ = newValue; }
	inline void updateMaxZ(float newValue) { maxZ = newValue; }

	/** Getters */
	inline float getMinX() { return minX; }
	inline float getMaxX() { return maxX; }

	inline float getMinY() { return minY; }
	inline float getMaxY() { return maxY; }

	inline float getMinZ() { return minZ; }
	inline float getMaxZ() { return maxZ; }

private:
	/** Render data */
	GLuint VBO;
	GLuint wiredEBO;

	GLuint nrIndicesWired;

	std::vector<Vertex> verticesData;

	// min/max values
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
};

