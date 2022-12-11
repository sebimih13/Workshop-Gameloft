#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"

#include "../Utilities/NFG.h"

Model::Model(ModelResource* modelResource) : resource(modelResource)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &wiredEBO);
}

Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &wiredEBO);
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
	std::vector<unsigned int> indicesData = data->indices;

	// TODO : ???
	delete data;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO : nrIndiciWired
	std::vector<unsigned int> wiredIndices;
	for (unsigned int i = 0; i < nrIndices; i += 3)
	{
		wiredIndices.push_back(indicesData[i]);
		wiredIndices.push_back(indicesData[i + 1]);

		wiredIndices.push_back(indicesData[i]);
		wiredIndices.push_back(indicesData[i + 2]);

		wiredIndices.push_back(indicesData[i + 1]);
		wiredIndices.push_back(indicesData[i + 2]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wiredIndices.size() * sizeof(unsigned int), &wiredIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	nrIndicesWired = wiredIndices.size();
}

