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

bool debugScene = false;
bool grayscaleScene = false;
bool blurScene = false;
bool sharpenScene = false;

int Init ( ESContext *esContext )
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor (	SceneManager::getInstance()->getBackgroundColor().x, 
					SceneManager::getInstance()->getBackgroundColor().y, 
					SceneManager::getInstance()->getBackgroundColor().z, 
					1.0f);

	// Load our objects
	SceneManager::getInstance()->Load();

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

		// Update Objects
		SceneManager::getInstance()->Update(deltaTime);

		// Update components
		camera->setDeltaTime(deltaTime);

		// Activate Blur Mode
		if (input[ControlsConfig::BLUR_MODE])
		{
			blurScene = !blurScene;
			input[ControlsConfig::BLUR_MODE] = false;

			if (blurScene)
			{
				std::cout << "BLUR MODE ACTIVATED\n";
			}
			else
			{
				std::cout << "BLUR MODE DEACTIVATED\n";
			}

			SceneManager::getInstance()->blurScene(blurScene);
		}

		// Activate Sharpen Mode
		if (input[ControlsConfig::SHARPEN_MODE])
		{
			sharpenScene = !sharpenScene;
			input[ControlsConfig::SHARPEN_MODE] = false;

			if (sharpenScene)
			{
				std::cout << "SHARPEN MODE ACTIVATED\n";
			}
			else
			{
				std::cout << "SHARPEN MODE DEACTIVATED\n";
			}

			SceneManager::getInstance()->sharpenScene(sharpenScene);
		}

		// Activate Grayscale Mode
		if (input[ControlsConfig::GRAYSCALE_MODE])
		{
			grayscaleScene = !grayscaleScene;
			input[ControlsConfig::GRAYSCALE_MODE] = false;

			if (grayscaleScene)
			{
				std::cout << "GRAYSCALE MODE ACTIVATED\n";
			}
			else
			{
				std::cout << "GRAYSCALE MODE DEACTIVATED\n";
			}

			SceneManager::getInstance()->grayscaleScene(grayscaleScene);
		}

		// Activate Debug Mode
		if (input[ControlsConfig::DEBUG_MODE])
		{
			debugScene = !debugScene;
			input[ControlsConfig::DEBUG_MODE] = false;

			if (debugScene)
			{
				std::cout << "DEBUG MODE ACTIVATED\n";
			}
			else
			{
				std::cout << "DEBUG MODE DEACTIVATED\n";
			}

			SceneManager::getInstance()->debugScene(debugScene);
		}

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
	// TODO : SceneManager::Controls => static
	
	// Update input
	std::map<char, ControlsConfig> inputMap = SceneManager::getInstance()->getControls();
	input[inputMap[key]] = bIsPressed;
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
	// TODO : delete stuffs
	SceneManager::getInstance()->Clear();
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
    
	* 4. In SceneManager / ResourceManager - variabile / functii static ?

	* 7. structurile ModelResource / TextureResource / ShaderResource - ar trb sa mai memoreze id?
		 * id-ul este folosit doar de ResourceManager cand adauga resursa in map[id] = resursa  
		 * RASPUNS : nu mai trb

	* 11. Daca avem mai multe variabile de setat inainte de Init()		- vezi exemplu Camera.h
		  * V1 - avem setters si dupa apelam o functie Init()
		  * V2 - avem un constructor toti parametrii care contine la sfarsit functia Init()

	* 12. Camera ar trb limitata pe rotatia OX sa nu poata sa faca 360
		* De revizuit mai tarziu

	* 15. in esUtil_win.cpp : 
	  
	  case WM_MOUSEMOVE: 
      {
          ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

          if (esContext && esContext->mouseFunc)
              esContext->mouseFunc(esContext, MouseButtons::left, MouseEvents::click, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      }
      break;


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI TEREN									-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  * 19. pot sa folosesc fs_uv_2 in terrainShader.vs ? 


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI LUMINI								-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  24. Exista posibilitatea sa transmit numarul de lumini si sa am acel for ?
			-> problema : in fragment shader trb sa fie const pentru a putea itera


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI COLIZIUNI								-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  26. Este gresit calculul minX, maxX, minY, maxY, minZ, maxZ ?


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI TRAIECTORII							-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  28. Traiectoria pt circle = ?


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI DEBUG SCENE							-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  29. Normalele pt fiecare vertex
		  Trb calculate manual si dupa trimise catre vertex+fragment shaders




	 ! De citit cap 10  OpenGL 20ES/Addison.Wesley.OpenGL.ES.2.0.Programming.Guide.Aug.2008.pdf

     3. de ridicat skybox 
     4. teren mai mare + fog + lumini

	 *. outlining pt obiecte
*/

/*

	TODO LIST : 

	* 1. ObjectType type -> conteaza doar la parsare => sterge din proprietatile unui SceneObject

	* 3. Forward Declarations in toate fisierele

	* 4. rotate camera - mouse input

	* 5. ObjectType type -> conteaza doar la parsare => sterge din proprietatile unui SceneObject

	* 6. CONTROLS -> MERGI IN SUS/JOS GLOBAL (NU RELATIV CUM ESTE ACM)
				  -> MERGI IN INAINTE/INAPOI GLOBAL (NU RELATIV CUM ESTE ACM)

	* 7. pt fiecare resource -> sa fie data ca parametru in Init()

	* 8. pt ceata - fara if => clamp

	* 9. verifica fiecare pointer inainte de a-l folosi

	* 10. Circle trajectory

	* 11. 

*/

