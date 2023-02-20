#include "stdafx.h"
#include "CollisionComponent.h"

#include "Vertex.h"

CollisionComponent::CollisionComponent(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &wiredEBO);

	std::vector<Vertex> verticesData = {
		Vertex(Vector3(minX, minY, maxZ)),		// 0
		Vertex(Vector3(maxX, minY, maxZ)),		// 1
		Vertex(Vector3(minX, minY, minZ)),		// 2
		Vertex(Vector3(maxX, minY, minZ)),		// 3

		Vertex(Vector3(minX, maxY, maxZ)),		// 4
		Vertex(Vector3(maxX, maxY, maxZ)),		// 5
		Vertex(Vector3(minX, maxY, minZ)),		// 6
		Vertex(Vector3(maxX, maxY, minZ)),		// 7
	};

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

