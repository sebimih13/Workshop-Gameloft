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
	void Draw(Shaders* programShader);

	/** Getters / Setters */
	inline void SetMVP(Matrix MVPtoSet) { MVP = MVPtoSet; }

private:
	/** Mesh data */
	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indices;

	/** Matrix */
	Matrix MVP;

	/** Render data */
	GLuint VBO, EBO;
	GLuint textureID;
};

