#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

#include "Vertex.h"

struct ModelResource
{
	int id;					// TODO : do i need this?
	std::string filePath;
	std::string fileName;
};

class Model
{
public:
	/** Constructor */
	Model();

	/** Destructor */
	~Model();

	/** Load model from ModelResource */
	void Load(ModelResource* modelResource);

	/** Load buffers */
	void LoadBuffers(std::vector<Vertex>& verticesData, std::vector<unsigned int>& indicesData);

	/** Getters / Setters */
	inline GLuint getVBO() { return VBO; }
	inline GLuint getEBO() { return EBO; }
	inline GLuint getWiredEBO() { return wiredEBO; }

	inline GLuint getNrIndices() { return nrIndices; }
	inline GLuint getNrIndicesWired() { return nrIndicesWired; }

private:
	GLuint VBO;
	GLuint EBO;
	GLuint wiredEBO;

	GLuint nrIndices, nrIndicesWired;
};

