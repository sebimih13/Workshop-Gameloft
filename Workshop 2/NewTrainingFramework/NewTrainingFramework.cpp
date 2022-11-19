// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"

#include "Camera.h"
#include "Mesh.h"

GLuint vboId;
Shaders myShaders;

float updateTimer = 0.05f;
float timer = 0.0f;

// Camera
Camera camera(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 5.0f));

// Model Loader
Mesh mesh;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[3];

	verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// TODO : load model
	mesh.Init("../ResourcesPacket/Models/Croco.nfg");

	std::cout << "aici\n";

	int index = 0;
	for (auto& v : mesh.verticesData)
	{
		std::cout << index++ << " : " << v.pos.x << ' ' << v.pos.y << ' ' << v.pos.z << "\n";
	}

	std::cout << "\n\n";

	index = 0;
	for (int i = 0; i < mesh.indices.size(); i += 3)
	{
		std::cout << index++ << " : " << mesh.indices[i] << ' ' << mesh.indices[i + 1] << ' ' << mesh.indices[i + 2] << "\n";
	}


	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);


	// Mesh start
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

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

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Mesh end



	//glBindBuffer(GL_ARRAY_BUFFER, vboId);

	//if (myShaders.positionAttribute != -1)
	//{
	//	glEnableVertexAttribArray(myShaders.positionAttribute);
	//	glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//}

	//if (myShaders.colorAttribute != -1)
	//{
	//	glEnableVertexAttribArray(myShaders.colorAttribute);
	//	glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	//}

	//if (myShaders.mvpMatrixUniform != -1)
	//{
	//	Matrix model;
	//	model.SetIdentity();

	//	Matrix MVP;
	//	MVP = model * camera.getViewMatrix() * camera.getProjectionMatrix();

	//	glUniformMatrix4fv(myShaders.mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	//}

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	timer += deltaTime;
	if (timer >= updateTimer)
	{
		timer = 0.0f;
		
		camera.setDeltaTime(deltaTime);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	std::cout << key << std::endl;
	std::cout << bIsPressed << std::endl << std::endl;

	// TODO : ESC key => exit

	if (bIsPressed)
	{
		switch (key)
		{
			case 'W': camera.moveOz(-1.0f);	break;
			case 'S': camera.moveOz(1.0f);	break;
			case 'A': camera.moveOx(-1.0f); break;
			case 'D': camera.moveOx(1.0f);	break;

			case '&': std::cout << "SUS\n";			 break;
			case '(': std::cout << "JOS\n";			 break;
			case '%': std::cout << "STANGA\n";		 break;
			case '\'': std::cout << "DREAPTA\n";	camera.rotateOy(); break;
		}
	}
}

void Mouse(ESContext* esContext, bool leftClick, int eventType, int mouseX, int mouseY)		// ??? coordonate float sau int ???
{
	std::cout << mouseX << ' ' << mouseY << "\n";
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Workshop", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

