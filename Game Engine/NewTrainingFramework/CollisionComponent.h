#pragma once

#include "stdafx.h"

#include "../Utilities/utilities.h"

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

private:
	/** Render data */
	GLuint VBO;
	GLuint wiredEBO;

	GLuint nrIndicesWired;
};

