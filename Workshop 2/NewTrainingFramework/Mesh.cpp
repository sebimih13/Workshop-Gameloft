#include "stdafx.h"	
#include "Mesh.h"

#include <fstream>
#include "../Utilities/utilities.h"

#include <sstream>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::Init(char* file)
{
	std::ifstream fin(file);
	if (fin.good())
	{
		std::cout << "VALID\n";
	}
	else
	{
		std::cout << "INVALID\n";
		return;
	}

	int nrVertices;
	std::string extra;
	fin >> extra >> nrVertices;
	fin.get();
	std::cout << extra << " : " << nrVertices << '\n';

	// Read vertices : pos + norm + binorm + tgt + uv
	for (int i = 0; i < nrVertices; i++)
	{
		std::string line;
		getline(fin, line);

		std::stringstream modifiedLine;

		for (char c : line)
			if (('0' <= c && c <= '9') || c == '-' || c == '.' || c == ' ')
				modifiedLine << c;
				
		Vector3 pos;
		Vector3 norm;
		Vector3 binorm;
		Vector3 tgt;
		Vector2 uv;

		modifiedLine >> pos.x >> pos.x >> pos.y >> pos.z;
		modifiedLine >> norm.x >> norm.y >> norm.z;
		modifiedLine >> binorm.x >> binorm.y >> binorm.z;
		modifiedLine >> tgt.x >> tgt.y >> tgt.z;
		modifiedLine >> uv.x >> uv.y;

		// TODO : DEBUG
		//std::cout << modifiedLine.str() << "\n";
		//std::cout << pos.x << ' ' << pos.y << ' ' << pos.z << "\n";
		//std::cout << norm.x << ' ' << norm.y << ' ' << norm.z << "\n";
		//std::cout << binorm.x << ' ' << binorm.y << ' ' << binorm.z << "\n";
		//std::cout << tgt.x << ' ' << tgt.y << ' ' << tgt.z << "\n";
		//std::cout << uv.x << ' ' << uv.y << ' ' << "\n\n\n";

		verticesData.push_back(pos.x);
		verticesData.push_back(pos.y);
		verticesData.push_back(pos.z);
	}

	int nrIndices;
	fin >> extra >> nrIndices;
	fin.get();
	std::cout << extra << " : " << nrIndices << '\n';

	// Read indices : 1. + 2. + 3.
	for (int i = 0; i < nrIndices / 3; i++)
	{
		std::string line;
		getline(fin, line);

		std::stringstream modifiedLine;

		for (char c : line)
			if (('0' <= c && c <= '9') || c == ' ')
				modifiedLine << c;
		
		Vector3 indices3;
		modifiedLine >> indices3.x >> indices3.x >> indices3.y >> indices3.z;

		// TODO : DEBUG
		//std::cout << modifiedLine.str() << '\n';
		//std::cout << indices.x << ' ' << indices.y << ' ' << indices.z << "\n\n";

		indices.push_back(indices3.x);
		indices.push_back(indices3.y);
		indices.push_back(indices3.z);
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	// ---------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);

	// ----------------------------------------------------

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	if (myShaders.mvpMatrixUniform != -1)
	{
		Matrix model;
		model.SetIdentity();

		Matrix MVP;
		MVP = model * camera.getViewMatrix() * camera.getProjectionMatrix();

		glUniformMatrix4fv(myShaders.mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw()
{

}

