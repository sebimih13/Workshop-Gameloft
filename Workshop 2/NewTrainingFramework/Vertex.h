#pragma once
#include "Math.h"

struct Vertex 
{
	/** Constructor */
	Vertex() : pos(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f) {}

	/** Copy constructor */
	Vertex(const Vertex& rhs) : pos(rhs.pos), color(rhs.color) {}

	Vector3 pos;
	Vector3 color;
};

