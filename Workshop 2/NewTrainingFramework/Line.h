#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"

#include "Shaders.h"
#include <vector>

class Line
{
public:
	/** Constructors */
	Line();

	/** Destructors */
	~Line();

	/** Init */
	void Init(Vector3 startPoint, Vector3 endPoint);

	/** Draw */
	void Draw();

private:
	/** Mesh data */
	std::vector<Vertex> verticesData;

	Shaders programShader;

	/** Matrix */
	Matrix MVP;

	/** Render data */
	GLuint VBO;
};

