#pragma once

#include "Math.h"

struct Vertex 
{
	/** Constructor */
	Vertex() :	pos(0.0f, 0.0f, 0.0f),
				norm(0.0f, 0.0f, 0.0f),
				binorm(0.0f, 0.0f, 0.0f),
				tgt(0.0f, 0.0f, 0.0f),
				uv(0.0f, 0.0f) 
	{  }

	Vertex(Vector3 position) :	pos(position),
								norm(0.0f, 0.0f, 0.0f),
								binorm(0.0f, 0.0f, 0.0f),
								tgt(0.0f, 0.0f, 0.0f),
								uv(0.0f, 0.0f)
	{  }

	Vertex(Vector3 position, Vector2 uv) :	pos(position),
											norm(0.0f, 0.0f, 0.0f),
											binorm(0.0f, 0.0f, 0.0f),
											tgt(0.0f, 0.0f, 0.0f),
											uv(uv)
	{  }

	/** Copy Constructor */
	Vertex(const Vertex& rhs) : pos(rhs.pos), 
								norm(rhs.norm),
								binorm(rhs.binorm),
								tgt(rhs.tgt),
								uv(rhs.uv) 
	{  }

	Vector3 pos;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;
};

