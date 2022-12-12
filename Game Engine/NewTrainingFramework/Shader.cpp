#include "stdafx.h"
#include "Shader.h"
#include "Vertex.h"

Shader::Shader(ShaderResource* shaderResource) : MAX_TEXTURES(5), resource(shaderResource)
{
	textureUniforms.resize(MAX_TEXTURES, -1);
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

	// Finding location of uniforms / attributes

	// Any scene object
	positionAttribute = glGetAttribLocation(programID, "a_pos");
	uvAttribute = glGetAttribLocation(programID, "a_uv");

	mvpMatrixUniform = glGetUniformLocation(programID, "u_mvpMatrix");
	colorUniform = glGetUniformLocation(programID, "u_color");

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		std::string uniformName = "u_texture_" + std::to_string(i);
		textureUniforms[i] = glGetUniformLocation(programID, uniformName.c_str());
	}

	// Terrain Uniforms
	heightUniform = glGetUniformLocation(programID, "u_height");
	nrCeluleUniform = glGetUniformLocation(programID, "u_nrCelule");
	offsetXUniform = glGetUniformLocation(programID, "u_offsetX");
	offsetZUniform = glGetUniformLocation(programID, "u_offsetZ");
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
		glUniform3fv(colorUniform, 1, &color->x);
	}
}

void Shader::setTexture(GLint index)
{
	if (textureUniforms[index] != -1)
	{
		glUniform1i(textureUniforms[index], index);
	}
}

void Shader::setNrCelule(GLint nrCelule)
{
	if (nrCeluleUniform != -1)
	{
		glUniform1i(nrCeluleUniform, nrCelule);
	}
}

void Shader::setHeight(Vector3* height)
{
	if (heightUniform != -1)
	{
		glUniform3fv(heightUniform, 1, &height->x);
	}
}

void Shader::setOffsetX(GLint offset)
{
	if (offsetXUniform != -1)
	{
		glUniform1i(offsetXUniform, offset);
	}
}

void Shader::setOffsetZ(GLint offset)
{
	if (offsetZUniform != -1)
	{
		glUniform1i(offsetZUniform, offset);
	}
}

