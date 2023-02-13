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

	// Attributes
	positionAttribute = glGetAttribLocation(programID, "a_pos");
	normalAttribute = glGetAttribLocation(programID, "a_norm");
	binormalAttribute = glGetAttribLocation(programID, "a_binorm");
	tangentAttribute = glGetAttribLocation(programID, "a_tgt");
	uvAttribute = glGetAttribLocation(programID, "a_uv");

	// Uniforms
	mvpMatrixUniform = glGetUniformLocation(programID, "u_mvpMatrix");
	modelMatrixUniform = glGetUniformLocation(programID, "u_modelMatrix");

	colorUniform = glGetUniformLocation(programID, "u_color");

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		std::string uniformName = "u_texture_" + std::to_string(i);
		textureUniforms[i] = glGetUniformLocation(programID, uniformName.c_str());
	}

	// Light position
	lightPositionUniform = glGetUniformLocation(programID, "u_lightPos");

	// Camera position
	viewPositionUniform = glGetUniformLocation(programID, "u_viewPos");
}

void Shader::setPosition()
{
	if (positionAttribute != -1)
	{
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
}

void Shader::setNormal()
{
	if (normalAttribute != -1)
	{
		glEnableVertexAttribArray(normalAttribute);
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(1 * sizeof(Vector3)));
	}
}

void Shader::setBinormal()
{
	if (binormalAttribute != -1)
	{
		glEnableVertexAttribArray(binormalAttribute);
		glVertexAttribPointer(binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}
}

void Shader::setTangent()
{
	if (tangentAttribute != -1)
	{
		glEnableVertexAttribArray(tangentAttribute);
		glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Vector3)));
	}
}

void Shader::setUV()
{
	if (uvAttribute != -1)
	{
		glEnableVertexAttribArray(uvAttribute);
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(Vector3)));
	}
}

void Shader::setMVP(Matrix* MVP)
{
	if (mvpMatrixUniform != -1)
	{
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP->m);
	}
}

void Shader::setModelMatrixUniform(Matrix* model)
{
	if (modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, (GLfloat*)model->m);
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

void Shader::setCameraViewPosition(Vector3* pos)
{
	if (viewPositionUniform != -1)
	{
		glUniform3fv(viewPositionUniform, 1, &pos->x);
	}
}

