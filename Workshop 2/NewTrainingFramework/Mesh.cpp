#include "stdafx.h"	
#include "Mesh.h"

#include "Shaders.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &textureID);
}

void Mesh::Init(char* file)
{
	std::ifstream fin(file);
	if (fin.good())
	{
		std::cout << "VALID : MESH\n";
	}
	else
	{
		std::cout << "INVALID : MESH\n";
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

		Vertex v;

		modifiedLine >> v.pos.x >> v.pos.x >> v.pos.y >> v.pos.z;
		modifiedLine >> norm.x >> norm.y >> norm.z;
		modifiedLine >> binorm.x >> binorm.y >> binorm.z;
		modifiedLine >> tgt.x >> tgt.y >> tgt.z;
		modifiedLine >> v.uv.x >> v.uv.y;

		// TODO : DEBUG
		//std::cout << modifiedLine.str() << "\n";
		//std::cout << v.pos.x << ' ' << v.pos.y << ' ' << v.pos.z << "\n";
		//std::cout << norm.x << ' ' << norm.y << ' ' << norm.z << "\n";
		//std::cout << binorm.x << ' ' << binorm.y << ' ' << binorm.z << "\n";
		//std::cout << tgt.x << ' ' << tgt.y << ' ' << tgt.z << "\n";
		//std::cout << v.uv.x << ' ' << v.uv.y << ' ' << "\n\n\n";

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
		//std::cout << modifiedLine.str() << '\n';
		//std::cout << ind1 << ' ' << ind2 << ' ' << ind3 << "\n\n";

		indices.push_back(ind1);
		indices.push_back(ind2);
		indices.push_back(ind3);
	}

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO : load textures
	int textureWidth, textureHeight, texureBpp;
	char* textureCroco = LoadTGA("../ResourcesPacket/Textures/Croco.tga", &textureWidth, &textureHeight, &texureBpp);

	std::cout << "textureWidth : " << textureWidth << "\n";
	std::cout << "textureHeight : " << textureHeight << "\n";
	std::cout << "texureBpp : " << texureBpp << "\n";

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureCroco);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::Draw(Shaders programShader)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (programShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(programShader.positionAttribute);
		glVertexAttribPointer(programShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (programShader.colorAttribute != -1)
	{
		glEnableVertexAttribArray(programShader.colorAttribute);
		glVertexAttribPointer(programShader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	if (programShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(programShader.uvAttribute);
		glVertexAttribPointer(programShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}

	if (programShader.mvpMatrixUniform != -1)
	{
		glUniformMatrix4fv(programShader.mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

