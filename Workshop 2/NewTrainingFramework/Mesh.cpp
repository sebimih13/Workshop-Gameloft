#include "stdafx.h"	
#include "Mesh.h"

#include <fstream>
#include <sstream>

#include "Shaders.h"

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
				
		Vector3 norm;
		Vector3 binorm;
		Vector3 tgt;
		Vector2 uv;

		Vertex v;

		modifiedLine >> v.pos.x >> v.pos.x >> v.pos.y >> v.pos.z;
		modifiedLine >> norm.x >> norm.y >> norm.z;
		modifiedLine >> binorm.x >> binorm.y >> binorm.z;
		modifiedLine >> tgt.x >> tgt.y >> tgt.z;
		modifiedLine >> uv.x >> uv.y;

		// TODO : DEBUG
		std::cout << modifiedLine.str() << "\n";
		std::cout << v.pos.x << ' ' << v.pos.y << ' ' << v.pos.z << "\n";
		std::cout << norm.x << ' ' << norm.y << ' ' << norm.z << "\n";
		std::cout << binorm.x << ' ' << binorm.y << ' ' << binorm.z << "\n";
		std::cout << tgt.x << ' ' << tgt.y << ' ' << tgt.z << "\n";
		std::cout << uv.x << ' ' << uv.y << ' ' << "\n\n\n";

		v.color.x = 1.0f;
		v.color.y = 1.0f;
		v.color.z = 1.0f;

		verticesData.push_back(v);
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
		
		unsigned int ind1, ind2, ind3;
		modifiedLine >> ind1 >> ind1;
		modifiedLine >> ind2;
		modifiedLine >> ind3;

		// TODO : DEBUG
		std::cout << modifiedLine.str() << '\n';
		std::cout << ind1 << ' ' << ind2 << ' ' << ind3 << "\n\n";

		indices.push_back(ind1);
		indices.push_back(ind2);
		indices.push_back(ind3);
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw(Shaders programShader)
{
	// TODO
	
}

