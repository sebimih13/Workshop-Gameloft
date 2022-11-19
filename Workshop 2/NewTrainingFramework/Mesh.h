#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"

#include <vector>

/** Forward Declarations */
class Shaders;

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
	void Draw(Shaders programShader);

// todo : private:
	/** Mesh data */
	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indices;

	/** Render data */
	GLuint VBO, EBO;
};

