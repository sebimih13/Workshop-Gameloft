#include "stdafx.h"
#include "Shader.h"
#include "Vertex.h"

Shader::Shader(ShaderResource* shaderResource) : resource(shaderResource)
{

}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::Load()
{
	std::string fileVertexShader = resource->filePath + resource->fileVS;
	GLuint vertexShader = esLoadShader(GL_VERTEX_SHADER, (char*)(fileVertexShader.c_str()));
	if (vertexShader == 0)
	{
		std::cout << "ERROR : VERTEX SHADER\n";
		return;
	}

	std::string fileFragmentShader = resource->filePath + resource->fileFS;
	GLuint fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, (char*)(fileFragmentShader.c_str()));
	if (fragmentShader == 0)
	{
		std::cout << "ERROR : FRAGMENT SHADER\n";
		return;
	}

	programID = esLoadProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(programID, "a_pos");
	uvAttribute = glGetAttribLocation(programID, "a_uv");

	mvpMatrixUniform = glGetUniformLocation(programID, "u_mvpMatrix");
	colorUniform = glGetUniformLocation(programID, "u_color");
}

void Shader::setPosition()
{
	if (positionAttribute != -1)
	{
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
}

void Shader::setUV()
{
	if (uvAttribute != -1)
	{
		glEnableVertexAttribArray(uvAttribute);
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}
}

void Shader::setMVP(Matrix* MVP)
{
	if (mvpMatrixUniform != -1)
	{
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP->m);
	}
}

void Shader::setColor(Vector3* color)
{
	if (colorUniform != -1)
	{
		glUniform3fv(colorUniform, 1, &color->x);		// TODO : check
	}
}

