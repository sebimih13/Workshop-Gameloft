// NewTrainingFramework.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "../Utilities/utilities.h"		// if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>

#include "SceneManager.h"
#include "ResourceManager.h"

// Update timer
float updateTimer = 0.01f;
float timer = 0.0f;

// Camera
Camera* camera;

// Input
std::map<ControlsConfig, bool> input;

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);

	glClearColor (	SceneManager::getInstance()->getBackgroundColor().x, 
					SceneManager::getInstance()->getBackgroundColor().y, 
					SceneManager::getInstance()->getBackgroundColor().z, 
					1.0f);

	// Load our objects
	SceneManager::getInstance()->LoadObjects();

	// Get active camera
	camera = SceneManager::getInstance()->getActiveCamera();

	// 0 Errors
	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw scene
	SceneManager::getInstance()->Draw();

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{		
	timer += deltaTime;
	if (timer >= updateTimer)
	{
		timer = 0.0f;

		// Update components
		camera->setDeltaTime(deltaTime);

		// Update Input

		// Move camera
		if (input[ControlsConfig::MOVE_CAMERA_POSITIVE_X])	// todo : -1.0f -> 1.0f
			camera->moveOx(-1.0f);
		if (input[ControlsConfig::MOVE_CAMERA_NEGATIVE_X])	// todo : 1.0f -> -1.0f
			camera->moveOx(1.0f);

		if (input[ControlsConfig::MOVE_CAMERA_POSITIVE_Y])
			camera->moveOy(1.0f);
		if (input[ControlsConfig::MOVE_CAMERA_NEGATIVE_Y])
			camera->moveOy(-1.0f);

		if (input[ControlsConfig::MOVE_CAMERA_POSITIVE_Z])	// todo : -1.0f -> 1.0f
			camera->moveOz(-1.0f);
		if (input[ControlsConfig::MOVE_CAMERA_NEGATIVE_Z])	// todo : 1.0f -> -1.0f
			camera->moveOz(1.0f);

		// Rotate camera
		if (input[ControlsConfig::ROTATE_CAMERA_POSITIVE_X])
			camera->rotateOx(1.0f);
		if (input[ControlsConfig::ROTATE_CAMERA_NEGATIVE_X])
			camera->rotateOx(-1.0f);

		if (input[ControlsConfig::ROTATE_CAMERA_POSITIVE_Y])
			camera->rotateOy(1.0f);
		if (input[ControlsConfig::ROTATE_CAMERA_NEGATIVE_Y])
			camera->rotateOy(-1.0f);

		if (input[ControlsConfig::ROTATE_CAMERA_POSITIVE_Z])
			camera->rotateOz(1.0f);
		if (input[ControlsConfig::ROTATE_CAMERA_NEGATIVE_Z])
			camera->rotateOz(-1.0f);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	// TODO : ESC key => exit
	// TODO : SceneManager::Controls
	
	// Update input
	std::map<char, ControlsConfig> inputMap = SceneManager::getInstance()->getControls();
	input[inputMap[key]] = bIsPressed;

	//TODO : delete?
	/*
	if (bIsPressed)
	{
		switch (key)
		{
			case 'W':	camera->moveOz(-1.0f);		break;
			case 'S':	camera->moveOz(1.0f);		break;
			case 'A':	camera->moveOx(-1.0f);		break;
			case 'D':	camera->moveOx(1.0f);		break;

			case 'Q':	camera->moveOy(1.0f);		break;
			case 'E':	camera->moveOy(-1.0f);		break;


			case 'O':	camera->rotateOz(1.0f);		break;
			case 'P':	camera->rotateOz(-1.0f);	break;

			case '&':	camera->rotateOx(1.0f);		break;
			case '(':	camera->rotateOx(-1.0f);	break;
			case '%':	camera->rotateOy(1.0f);		break;
			case '\'':	camera->rotateOy(-1.0f);	break;
			case 'O':	camera->rotateOz(1.0f);		break;
			case 'P':	camera->rotateOz(-1.0f);	break;
		}
	}
	*/
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
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	// Resource Manager
	ResourceManager::getInstance()->Init("../XMLFiles/resourceManager.xml");

	// Scene Manager
	SceneManager::getInstance()->Init("../XMLFiles/sceneManager.xml");

	// OpenGL ES
	ESContext esContext;
    esInitContext ( &esContext );

	esCreateWindow ( &esContext, SceneManager::getInstance()->getGameName().c_str(), 
					SceneManager::getInstance()->getDefaultScreenSize().width,
					SceneManager::getInstance()->getDefaultScreenSize().height,
					ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc ( &esContext, Mouse );

	esMainLoop ( &esContext );

	// releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

/**	INTREBARI : 
    
	* 1. Unde trb implementat MVP (SceneManager / SceneObject)
	     * SceneManager -> avem acces usor la camera.view + camera.projection
		 * SceneObject  -> avem acces la postion / rotation / scale (de care avem nevoie pentru matrix model)

	* 4. In SceneManager / ResourceManager - variabile / functii static ?

	* 5. Daca stiu ca o variabila va fi populata ulterior, mai trb sa ii atribui o valoare default ? (pt siguranta)

	* 7. structurile ModelResource / TextureResource / ShaderResource - ar trb sa mai memoreze id?
		 * id-ul este folosit doar de ResourceManager cand adauga resursa in map[id] = resursa  
		 * RASPUNS : nu mai trb

	* 8. Attributes + Uniform trb incluse in clasa Shader ?
		* DA
		 
		 * V2 - in SceneObject->Draw() : de ficare data avem:
				
				attribute = glGetAttribLocation(programID, "attributeString");

				glEnableVertexAttribArray(attribute);
				glVertexAttribPointer(attribute, ...);

	* 9. Attributes + Uniform trb incluse in clasa Shader ?
		* DA

	* 11. Daca avem mai multe variabile de setat inainte de Init()		- vezi exemplu Camera.h
		  * V1 - avem setters si dupa apelam o functie Init()
		  * V2 - avem un constructor toti parametrii care contine la sfarsit functia Init()

	* 12. Camera ar trb limitata pe rotatia OX sa nu poata sa faca 360

	* 13. ORICE implementare -> daca ma rotesc => rotatie pe axa OZ a obiectului
		* De revizuit mai tarziu

	* 14. Special characters in XML -> exemplu daca vreau: & - pt ARROW UP
		* XML special characters

	* 15. in esUtil_win.cpp : 
	  
	  case WM_MOUSEMOVE: 
      {
          ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

          if (esContext && esContext->mouseFunc)
              esContext->mouseFunc(esContext, MouseButtons::left, MouseEvents::click, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      }
      break;

	  * 16. Pentru modelele care nu au texturi fac alt shader?
		* Mai usor de folosit
			* DA

*/

/*

	TODO LIST : 

	* 1. modelul in format wired
		* de actualizat lista de indici	

	* 2. pentru SceneObject - color (sau textures)
		 + shader separat pt obiectele care au culoare (nu au texturi)

	* 3. rotate camera - mouse input

	* 4. pt teren => extindere clasa SceneObject
		* drawCommon() - deseneaza toate lucrurile comune pt orice obiect din scena

	* 5. 

*/

