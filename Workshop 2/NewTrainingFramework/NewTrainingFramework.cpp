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
#include "Line.h"

GLuint vboId;
Shaders myShaders;

float updateTimer = 0.05f;
float timer = 0.0f;

// Camera
Camera camera(Vector3(0.0f, 0.0f, -500.0f), Vector3(0.0f, 0.0f, 0.0f));

// Mesh
Mesh mesh;

// Line
Line line;

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	// triangle data (heap)
	Vertex verticesData[3];

	verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;

	// buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// TODO : init mesh
	mesh.Init("../ResourcesPacket/Models/Croco.nfg");

	// TODO : init line
	line.Init(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));

	// creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShaders.program);

	// Calculate MVP
	Matrix model;
	model.SetIdentity();

	Matrix MVP;
	MVP = model * camera.getViewMatrix() * camera.getProjectionMatrix();

	// Draw Mesh 
	mesh.SetMVP(MVP);
	mesh.Draw(&myShaders);

	// Draw Line
	line.Draw();

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
	//std::cout << key << std::endl;
	//std::cout << bIsPressed << std::endl << std::endl;

	// TODO : ESC key => exit

	if (bIsPressed)
	{
		switch (key)
		{
			case 'W':	camera.moveOz(-1.0f);	break;
			case 'S':	camera.moveOz(1.0f);	break;
			case 'A':	camera.moveOx(-1.0f);	break;
			case 'D':	camera.moveOx(1.0f);	break;

			case 'Q':	camera.moveOy(1.0f);	break;
			case 'E':	camera.moveOy(-1.0f);	break;


			case 'O':	camera.rotateOz(1.0f);  break;
			case 'P':	camera.rotateOz(-1.0f); break;

			case '&':	camera.rotateOx(1.0f);	break;
			case '(':	camera.rotateOx(-1.0f); break;
			case '%':	camera.rotateOy(1.0f);	break;
			case '\'':	camera.rotateOy(-1.0f); break;
		}
	}
}

void Mouse(ESContext* esContext, MouseButtons button, MouseEvents eventType, int mouseX, int mouseY)
{
	switch (button)
	{
		case MouseButtons::left:	std::cout << "LEFT ";	break;
		case MouseButtons::right:	std::cout << "RIGHT ";	break;
	}

	switch (eventType)
	{
		case MouseEvents::click:	std::cout << "CLICK\n";				break;
		case MouseEvents::unclick:	std::cout << "UNCLICK\n";			break;
		case MouseEvents::dclick:	std::cout << "DOUBLE CLICK\n";		break;
	}

	std::cout << "MOUSE : " << mouseX << ' ' << mouseY << "\n\n";
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

	// releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

/*

	PROBLEME :


*/

