#pragma once

#include "Vertex.h"
#include <vector>

class Mesh
{
public:
	/** Constructors */
	Mesh();

	/** Destructors */
	~Mesh();

	/** Initialization */
	void Init(char* file);

	/** Draw */
	void Draw();

private:
	/** Mesh data */
	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indices;

	/** Render data */
	GLuint VBO, EBO;
};

