#include "stdafx.h"
#include "TerrainObject.h"
#include "Vertex.h"
#include "ResourceManager.h"

TerrainObject::TerrainObject()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

TerrainObject::~TerrainObject()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void TerrainObject::generateModel(Vector3 initialPosition)
{
	Vector3 startPosition;
	startPosition.x = initialPosition.x - dimensiuneCelula * (nrCelule / 2);
	startPosition.z = initialPosition.z + dimensiuneCelula * (nrCelule / 2);

	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indicesData;

	for (int i = 0; i < nrCelule + 1; i++)
	{
		for (int j = 0; j < nrCelule + 1; j++)
		{
			Vertex v;
			v.pos.x = startPosition.x + dimensiuneCelula * j;
			v.pos.z = startPosition.z - dimensiuneCelula * i;
			v.pos.y = 0.0f;

			verticesData.push_back(v);
		}
	}

	for (int i = 0; i < nrCelule; i++)
	{
		for (int j = 0; j < nrCelule; j++)
		{
			unsigned int stangaSus  = i + nrCelule * i + j;
			unsigned int dreaptaSus = i + nrCelule * i + j + 1;
			unsigned int stangaJos  = (i + 1) + nrCelule * (i + 1) + j;
			unsigned int dreaptaJos = (i + 1) + nrCelule * (i + 1) + j + 1;

			// Triunghi superior
			indicesData.push_back(stangaSus);
			indicesData.push_back(dreaptaSus);
			indicesData.push_back(stangaJos);

			// Triunghi inferior
			indicesData.push_back(dreaptaSus);
			indicesData.push_back(dreaptaJos);
			indicesData.push_back(stangaJos);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TerrainObject::Load()
{
	// TODO : load model

	if (shader != 0)	// TODO : CHECK
		shader = ResourceManager::getInstance()->LoadShader(shaderID);

	for (int& id : textureIDs)
	{
		Texture* texture = ResourceManager::getInstance()->LoadTexture(id);
		textures.push_back(texture);
	}
}

void TerrainObject::Draw()
{
	// TODO : call parent method
	SceneObject::Draw();

	// TODO : add more ?
}

