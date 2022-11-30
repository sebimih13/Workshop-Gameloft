#pragma once

#include "Math.h"

struct Vertex 
{
	/** Constructor */
	Vertex() : pos(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {  }

	/** Copy Constructor */
	Vertex(const Vertex& rhs) : pos(rhs.pos), uv(rhs.uv) {}

	Vector3 pos;
	Vector2 uv;
};

