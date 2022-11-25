#include "stdafx.h"
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if (vertexShader == 0)
	{
		std::cout << "ERROR : VERTEX SHADER\n";
		return -1;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		std::cout << "ERROR : FRAGMENT SHADER\n";
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	// finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_color");
	uvAttribute = glGetAttribLocation(program, "a_uv");

	mvpMatrixUniform = glGetUniformLocation(program, "u_mvpMatrix");

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

