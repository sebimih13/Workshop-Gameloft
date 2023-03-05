#include "stdafx.h"
#include "CollisionComponent.h"

#include <limits>

CollisionComponent::CollisionComponent(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
	:	worldMinX(FLT_MAX), worldMaxX(FLT_MIN),
		worldMinY(FLT_MAX), worldMaxY(FLT_MIN),
		worldMinZ(FLT_MAX), worldMaxZ(FLT_MIN)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &wiredEBO);

	verticesData.push_back(Vertex(Vector3(minX, minY, maxZ)));		// 0
	verticesData.push_back(Vertex(Vector3(maxX, minY, maxZ)));		// 1
	verticesData.push_back(Vertex(Vector3(minX, minY, minZ)));		// 2
	verticesData.push_back(Vertex(Vector3(maxX, minY, minZ)));		// 3

	verticesData.push_back(Vertex(Vector3(minX, maxY, maxZ)));		// 4
	verticesData.push_back(Vertex(Vector3(maxX, maxY, maxZ)));		// 5
	verticesData.push_back(Vertex(Vector3(minX, maxY, minZ)));		// 6
	verticesData.push_back(Vertex(Vector3(maxX, maxY, minZ)));		// 7

	std::vector<unsigned int> indicesData = {
		// patrat jos
		0, 1, 2,
		1, 2, 3,

		// patrat sus
		4, 5, 6,
		5, 6, 7, 

		// patrat dreapta
		1, 3, 5,
		3, 5, 7,

		// patrat stanga
		0, 2, 4,
		2, 4, 6,

		// patrat fata
		0, 1, 4,
		1, 4, 5,

		// patrat spate
		2, 3, 6, 
		3, 6, 7
	};
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// wired format
	std::vector<unsigned int> wiredIndices;
	for (unsigned int i = 0; i < indicesData.size(); i += 3)
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

CollisionComponent::~CollisionComponent()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &wiredEBO);
}

void CollisionComponent::CalculateVertexPosition(Matrix& modelMatrix)
{
	// MIN/MAX values
	float minX = FLT_MAX, maxX = FLT_MIN;
	float minY = FLT_MAX, maxY = FLT_MIN;
	float minZ = FLT_MAX, maxZ = FLT_MIN;

	for (Vertex& v : verticesData)
	{
		Vector4 vertexPosition = Vector4(v.pos, 1.0f) * modelMatrix;

		minX = min(minX, vertexPosition.x);
		maxX = max(maxX, vertexPosition.x);

		minY = min(minY, vertexPosition.y);
		maxY = max(maxY, vertexPosition.y);

		minZ = min(minZ, vertexPosition.z);
		maxZ = max(maxZ, vertexPosition.z);
	}

	worldMinX = minX;
	worldMaxX = maxX;

	worldMinY = minY;
	worldMaxY = maxY;

	worldMinZ = minZ;
	worldMaxZ = maxZ;
}

/*
	
	0 - (minX, minY, maxZ)
	1 - (maxX, minY, maxZ)
	2 - (minX, minY, minZ)
	3 - (maxX, minY, minZ)

	4 - (minX, maxY, maxZ)
	5 - (maxX, maxY, maxZ)
	6 - (minX, maxY, minZ)
	7 - (maxX, maxY, minZ)


	  6------7
     /|		/|
	4------5 |
	| |    | |
	| 2----|-3
	|/     |/
	0------1
	

*/

