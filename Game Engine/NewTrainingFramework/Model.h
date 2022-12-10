#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

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
	Model(ModelResource* modelResource);

	/** Destructor */
	~Model();

	/** Load model from .nfg */
	void Load();

	/** Getters / Setters */
	inline GLuint getVBO() { return VBO; }
	inline GLuint getEBO() { return EBO; }
	inline GLuint getWiredEBO() { return wiredEBO; }

	inline GLuint getNrIndices() { return nrIndices; }
	inline GLuint getNrIndicesWired() { return nrIndicesWired; }

private:
	ModelResource* resource;

	GLuint VBO;
	GLuint EBO;
	GLuint wiredEBO;

	GLuint nrIndices, nrIndicesWired;
};

