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
	normalAttribute = glGetAttribLocation(programID, "a_norm");
	uvAttribute = glGetAttribLocation(programID, "a_uv");

	mvpMatrixUniform = glGetUniformLocation(programID, "u_mvpMatrix");
	modelMatrixUniform = glGetUniformLocation(programID, "u_modelMatrix");

	colorUniform = glGetUniformLocation(programID, "u_color");

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		std::string uniformName = "u_texture_" + std::to_string(i);
		textureUniforms[i] = glGetUniformLocation(programID, uniformName.c_str());
	}

	// Ambiental Light
	ambientalLightColorUniform = glGetUniformLocation(programID, "u_ambientColor");
	ambientalLightStrengthUniform = glGetUniformLocation(programID, "u_ambientStrength");

	// Diffuse
	diffuseLightColorUniform = glGetUniformLocation(programID, "u_diffuseColor");
	diffuseLightStrengthUniform = glGetUniformLocation(programID, "u_diffuseStrength");

	// Specular
	specularLightColorUniform = glGetUniformLocation(programID, "u_specularColor");
	specularLightStrengthUniform = glGetUniformLocation(programID, "u_specularStrength");

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

void Shader::setLightPosition(Vector3* pos)
{
	if (lightPositionUniform != -1)
	{
		glUniform3fv(lightPositionUniform, 1, &pos->x);
	}
}

void Shader::setCameraViewPosition(Vector3* pos)
{
	if (viewPositionUniform != -1)
	{
		glUniform3fv(viewPositionUniform, 1, &pos->x);
	}
}

void Shader::setAmbientalLightColor(Vector3* color)
{
	if (ambientalLightColorUniform != -1)
	{
		glUniform3fv(ambientalLightColorUniform, 1, &color->x);
	}
}

void Shader::setAmbientalLightStrength(GLfloat strength)
{
	if (ambientalLightStrengthUniform != -1)
	{
		glUniform1f(ambientalLightStrengthUniform, strength);
	}
}

void Shader::setDiffuseLightColor(Vector3* color)
{
	if (diffuseLightColorUniform != -1)
	{
		glUniform3fv(diffuseLightColorUniform, 1, &color->x);
	}
}

void Shader::setDiffuseLightStrength(GLfloat strength)
{
	if (diffuseLightStrengthUniform != -1)
	{
		glUniform1f(diffuseLightStrengthUniform, strength);
	}
}

void Shader::setSpecularLightColor(Vector3* color)
{
	if (specularLightColorUniform != -1)
	{
		glUniform3fv(specularLightColorUniform, 1, &color->x);
	}
}

void Shader::setSpecularLightStrength(GLfloat strength)
{
	if (specularLightStrengthUniform != -1)
	{
		glUniform1f(specularLightStrengthUniform, strength);
	}
}

