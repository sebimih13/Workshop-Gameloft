#include "stdafx.h"
#include "TerrainObject.h"

#include "Camera.h"

TerrainObject::TerrainObject()
{
	// TODO : find a good value
	nrCelule = 4;				// TODO : nr par
	dimensiuneCelula = 100;	

	offsetX = 0;
	offsetY = 50;			// TODO : find a good value
	offsetZ = 0;
}

TerrainObject::~TerrainObject()
{
	
}

Model* TerrainObject::generateModel()
{
	position = camera->getPosition();

	Vector3 startPosition = Vector3(0.0f, 0.0f, 0.0f);
	startPosition.x = float(-dimensiuneCelula * (nrCelule / 2));
	startPosition.y = float(-offsetY);
	startPosition.z = float(-dimensiuneCelula * (nrCelule / 2));

	std::vector<Vertex> verticesData;
	std::vector<unsigned int> indicesData;

	for (int i = 0; i < nrCelule + 1; i++)
	{
		for (int j = 0; j < nrCelule + 1; j++)
		{
			Vertex v;
			v.pos.x = startPosition.x + dimensiuneCelula * j;
			v.pos.y = startPosition.y;
			v.pos.z = startPosition.z + dimensiuneCelula * i;

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
	// call parent method to load : shader + textures
	SceneObject::Load();

	// Load Uniforms
	heightUniform = glGetUniformLocation(shader->getProgramID(), "u_height");
	nrCeluleUniform = glGetUniformLocation(shader->getProgramID(), "u_nrCelule");
	offsetXUniform = glGetUniformLocation(shader->getProgramID(), "u_offsetX");
	offsetZUniform = glGetUniformLocation(shader->getProgramID(), "u_offsetZ");

	// load model manually
	model = generateModel();
}

void TerrainObject::Draw()
{
	glUseProgram(shader->getProgramID());

	// Set uniforms
	setNrCelule();
	setHeight();

	setOffsetX();
	setOffsetZ();

	// call parent method
	SceneObject::Draw();
}

void TerrainObject::Update(float deltaTime)
{
	// genereaza o noua bucata de teren pe axa X (dreapta/stanga)
	float deltaX = abs(position.x - camera->getPosition().x);
	if (deltaX > dimensiuneCelula)
	{
		if (camera->getPosition().x > position.x)		// deplasare la dreapta
		{
			position.x += dimensiuneCelula;
			offsetX++;
		}
		else if (camera->getPosition().x < position.x)	// deplasare la stanga
		{
			position.x -= dimensiuneCelula;
			offsetX--;
		}
	}

	// genereaza o noua bucata de teren pe axa Z (fata/spate)
	float deltaZ = abs(position.z - camera->getPosition().z);
	if (deltaZ > dimensiuneCelula)
	{
		if (camera->getPosition().z > position.z)		// deplasare inainte
		{
			position.z += dimensiuneCelula;
			offsetZ++;
		}
		else if (camera->getPosition().z < position.z)	// deplasare inapoi
		{
			position.z -= dimensiuneCelula;
			offsetZ--;
		}
	}
}

void TerrainObject::setNrCelule()
{
	if (nrCeluleUniform != -1)
	{
		glUniform1i(nrCeluleUniform, nrCelule);
	}
}

void TerrainObject::setHeight()
{
	if (heightUniform != -1)
	{
		glUniform3fv(heightUniform, 1, &height.x);
	}
}

void TerrainObject::setOffsetX()
{
	if (offsetXUniform != -1)
	{
		glUniform1i(offsetXUniform, offsetX % nrCelule);
	}
}

void TerrainObject::setOffsetZ()
{
	if (offsetZUniform != -1)
	{
		glUniform1i(offsetZUniform, offsetZ % nrCelule);
	}
}

