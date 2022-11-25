#include "stdafx.h"	

#include "Line.h"

Line::Line()
{
	
}

Line::~Line()
{
	glDeleteBuffers(1, &VBO);
}

void Line::Init(Vector3 startPoint, Vector3 endPoint)
{
	Vertex start;
	start.pos.x = startPoint.x;		start.pos.y = startPoint.y;		start.pos.z = startPoint.z;
	start.color.x = 1.0f;			start.color.y = 0.0f;			start.color.z = 0.0f;
	verticesData.push_back(start);

	Vertex end;
	end.pos.x = endPoint.x;			end.pos.y = endPoint.y;			end.pos.z = endPoint.z;
	end.color.x = 1.0f;				end.color.y = 0.0f;				end.color.z = 0.0f;
	verticesData.push_back(end);

	MVP.SetIdentity();

	if (programShader.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs") != 0)
	{
		std::cout << "INVALID : Line shaders not found\n";
	}
	else
	{
		std::cout << "VALID : Line shaders\n";
	}
		
	// Render data
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Line::Draw()
{
	glUseProgram(programShader.program);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

	if (programShader.mvpMatrixUniform != -1)
	{
		glUniformMatrix4fv(programShader.mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

