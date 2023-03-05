#include "stdafx.h"
#include "Model.h"

#include "../Utilities/NFG.h"

Model::Model()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &wiredEBO);

	nrIndices = 0;
	nrIndicesWired = 0;
}

Model::~Model()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &wiredEBO);
}

void Model::Load(ModelResource* resource)
{
	std::string file = resource->filePath + resource->fileName;	
	NFGData* data = LoadNFG(file.c_str());

	if (!data)
		return;

	for (int i = 0; i < data->nrVertices; i++)
	{
		Vertex v;
		v.pos = data->verticesData[i].pos;
		v.norm = data->verticesData[i].norm;
		v.binorm = data->verticesData[i].binorm;
		v.tgt = data->verticesData[i].tgt;
		v.uv  = data->verticesData[i].uv;

		verticesData.push_back(v);
	}

	LoadBuffers(verticesData, data->indices);

	delete data;
}

void Model::LoadBuffers(std::vector<Vertex>& verticesData, std::vector<unsigned int>& indicesData)
{
	this->verticesData = verticesData;

	nrIndices = indicesData.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndices * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// wired format
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

