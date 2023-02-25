#include "stdafx.h"
#include "Axes.h"

#include "Vertex.h"
#include "Shader.h"
#include "ResourceManager.h"

Axes::Axes(Vector3 colorOX, Vector3 colorOY, Vector3 colorOZ)
	: OXColor(colorOX), OYColor(colorOY), OZColor(colorOZ)
{
	
}

Axes::~Axes()
{
	glDeleteBuffers(1, &VBO_X);
	glDeleteBuffers(1, &VBO_Y);
	glDeleteBuffers(1, &VBO_Z);
}

void Axes::Load()
{
	// Generate buffers
	glGenBuffers(1, &VBO_X);
	glGenBuffers(1, &VBO_Y);
	glGenBuffers(1, &VBO_Z);

	// OX - Axis
	std::vector<Vertex> verticesData_X = {
		Vertex(Vector3(0.0, 0.0, 0.0)),		// 0
		Vertex(Vector3(1.0, 0.0, 0.0))		// X
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO_X);
	glBufferData(GL_ARRAY_BUFFER, verticesData_X.size() * sizeof(Vertex), &verticesData_X[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// OY - Axis
	std::vector<Vertex> verticesData_Y = {
		Vertex(Vector3(0.0, 0.0, 0.0)),		// 0
		Vertex(Vector3(0.0, 1.0, 0.0))		// Y
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Y);
	glBufferData(GL_ARRAY_BUFFER, verticesData_Y.size() * sizeof(Vertex), &verticesData_Y[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// OZ - Axis
	std::vector<Vertex> verticesData_Z = {
		Vertex(Vector3(0.0, 0.0, 0.0)),		// 0
		Vertex(Vector3(0.0, 0.0, 1.0))		// Z
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Z);
	glBufferData(GL_ARRAY_BUFFER, verticesData_Z.size() * sizeof(Vertex), &verticesData_Z[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Axes::Draw(Matrix* mvp)
{
	// Draw X-Axis	
	DrawAxis(Axis::OX, mvp);

	// Draw Y-Axis
	DrawAxis(Axis::OY, mvp);

	// Draw Z-Axis
	DrawAxis(Axis::OZ, mvp);
}

void Axes::DrawAxis(Axis axis, Matrix* mvp)
{
	GLuint* VBO = &VBO_X;
	Vector3* color = &OXColor;

	switch (axis)
	{
		case Axis::OX:
		{
			VBO = &VBO_X;
			color = &OXColor;
		}
		break;

		case Axis::OY:
		{
			VBO = &VBO_Y;
			color = &OYColor;
		}
		break;

		case Axis::OZ:
		{
			VBO = &VBO_Z;
			color = &OZColor;
		}
		break;
	}

	// Get Shader
	Shader* shader = ResourceManager::getInstance()->LoadShader(16);

	glUseProgram(shader->getProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	// Set Attributes
	shader->setPosition();

	// Set Uniforms
	shader->setMVP(mvp);
	shader->setColor(color);

	// Draw
	glDrawArrays(GL_LINES, 0, 2);

	// Unbind
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

