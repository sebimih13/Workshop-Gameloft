#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"

#include "../Utilities/NFG.h"

Model::Model(ModelResource* modelResource) : resource(modelResource)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// TODO : wiredEBO
}

Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// TODO : wiredEBO
}

void Model::Load()
{
	std::string file = resource->filePath + resource->fileName;	
	NFGData* data = LoadNFG(file.c_str());

	if (!data)
		return;

	std::vector<Vertex> verticesData;
	for (int i = 0; i < data->nrVertices; i++)
	{
		Vertex v;
		v.pos = data->verticesData[i].pos;
		v.uv  = data->verticesData[i].uv;

		verticesData.push_back(v);
	}

	nrIndices = data->nrIndices;
	std::vector<unsigned int> indices = data->indices;

	// TODO : ???
	delete data;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO : nrIndiciWired
}

