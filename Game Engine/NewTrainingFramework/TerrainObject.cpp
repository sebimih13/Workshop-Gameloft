#include "stdafx.h"
#include "TerrainObject.h"

#include "ResourceManager.h"
#include "Camera.h"

TerrainObject::TerrainObject()
{
	// TODO : change 
	nrCelule = 4;				// TODO : nr par
	dimensiuneCelula = 100;

	offsetX = 0;
	offsetY = 0;
	offsetZ = 0;
}

TerrainObject::~TerrainObject()
{
	
}

Model* TerrainObject::generateModel()
{
	currentPosition = camera->getPosition();

	Vector3 startPosition;
	startPosition.x = camera->getPosition().x - dimensiuneCelula * (nrCelule / 2);
	startPosition.z = camera->getPosition().z - dimensiuneCelula * (nrCelule / 2);

	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indicesData;

	for (int i = 0; i < nrCelule + 1; i++)
	{
		for (int j = 0; j < nrCelule + 1; j++)
		{
			Vertex v;
			v.pos.x = startPosition.x + dimensiuneCelula * j;
			v.pos.z = startPosition.z + dimensiuneCelula * i;
			v.pos.y = 0.0f;

			v.uv.x = float(j);
			v.uv.y = float(i);

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

	Model* generatedModel = new Model();
	generatedModel->LoadBuffers(verticesData, indicesData);

	return generatedModel;
}

void TerrainObject::Load()
{
	// call parent method to load shader + textures
	SceneObject::Load();

	// load model manually
	model = generateModel();
}

void TerrainObject::Draw()
{
	// TODO : mai trb ceva aditional?
	glUseProgram(shader->getProgramID());

	shader->setNrCelule(nrCelule);
	shader->setHeight(&height);

	shader->setOffsetX(offsetX % nrCelule);
	shader->setOffsetZ(offsetZ % nrCelule);

	// call parent method
	SceneObject::Draw();
}

void TerrainObject::Update()
{
	// TODO : o functie pt chestia asta cu parametrii : updatePosition(axa)

	// genereaza o noua bucata de teren pe axa X (dreapta/stanga)
	float deltaX = abs(currentPosition.x - camera->getPosition().x);
	if (deltaX > dimensiuneCelula)
	{
		if (camera->getPosition().x > currentPosition.x)		// deplasare la dreapta
		{
			position.x += dimensiuneCelula;
			currentPosition.x += dimensiuneCelula;
			offsetX++;
		}
		else if (camera->getPosition().x < currentPosition.x)	// deplasare la stanga
		{
			position.x -= dimensiuneCelula;
			currentPosition.x -= dimensiuneCelula;
			offsetX--;
		}
	}

	// genereaza o noua bucata de teren pe axa Z (fata/spate)
	float deltaZ = abs(currentPosition.z - camera->getPosition().z);
	if (deltaZ > dimensiuneCelula)
	{
		if (camera->getPosition().z > currentPosition.z)		// deplasare inainte
		{
			position.z += dimensiuneCelula;
			currentPosition.z += dimensiuneCelula;
			offsetZ++;
		}
		else if (camera->getPosition().z < currentPosition.z)	// deplasare inapoi
		{
			position.z -= dimensiuneCelula;
			currentPosition.z -= dimensiuneCelula;
			offsetZ--;
		}
	}
}

