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

		// Activate Debug Mode
		if (input[ControlsConfig::DEBUG_MODE])
		{
			debugScene = !debugScene;
			input[ControlsConfig::DEBUG_MODE] = false;

			std::cout << "DEBUG MODE ACTIVATED\n";

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
	// TODO : SceneManager::Controls
	
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

	* 5. Daca stiu ca o variabila va fi populata ulterior, mai trb sa ii atribui o valoare default ? (pt siguranta)

	* 7. structurile ModelResource / TextureResource / ShaderResource - ar trb sa mai memoreze id?
		 * id-ul este folosit doar de ResourceManager cand adauga resursa in map[id] = resursa  
		 * RASPUNS : nu mai trb

	* 11. Daca avem mai multe variabile de setat inainte de Init()		- vezi exemplu Camera.h
		  * V1 - avem setters si dupa apelam o functie Init()
		  * V2 - avem un constructor toti parametrii care contine la sfarsit functia Init()

	* 12. Camera ar trb limitata pe rotatia OX sa nu poata sa faca 360
		* De revizuit mai tarziu

	* 13. ORICE implementare -> daca ma rotesc => rotatie pe axa OZ a obiectului
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

	  * 16. LINEAR si REPEAT pt: ?
			- rock.tga
			- grass.tga
			- dirt.tga
			- Terrain_blend_map.tga

	  * 17. in terrainShader pun in .vs + .fs ?
				uniform sampler2D u_texture_3;	// terrain_blend_map.tga

	  * 19. pot sa folosesc fs_uv_2 in terrainShader.vs ? 

	  * 20. uniforms speciale pt un anumit tip de obiect:	* le pun in clasa obiectului ?
															* sau le pun pe toate la gramada in shader.h 
			ex. cele pt teren

	  
	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI CEATA									-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  * 21. in modelShader.fs - vec4(fogAlpha * u_fogColor, 1.0) - e 1.0 aici?


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI FOC									-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  * 22. Pt toate texturile avem?
	  		<min_filter>LINEAR</min_filter>
			<mag_filter>LINEAR</mag_filter>
			<wrap_s>REPEAT</wrap_s>		
			<wrap_t>REPEAT</wrap_t>	

	  * 23. 


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI LUMINI								-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  24. Exista posibilitatea sa transmit numarul de lumini si sa am acel for ?
			-> problema : in fragment shader trb sa fie const pentru a putea itera

	  25. In XML : 
			<associatedObject>2</associatedObject>			<!-- TODO : ASTA PT CE E? -->


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI COLIZIUNI								-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  26. Este gresit calculul minX, maxX, minY, maxY, minZ, maxZ ?

	  27. Este corect checkCollision ?


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI TRAIECTORII							-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  28. Traiectoria pt circle = ?


	  -----------------------------------------------------------------------------------------------------------------------------
	  -----------------------								INTREBARI DEBUG SCENE							-----------------------
	  -----------------------------------------------------------------------------------------------------------------------------

	  29. Normalele pt fiecare vertex
	      Exista geometry shader in OpenGL ES?
		  Trb calculate manual? si dupa trimise catre vertex+fragment shaders?


	  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  !!!!!!!!!!!!!!!!!!!!!!!									REZOLVA CAMERA								!!!!!!!!!!!!!!!!!!!!!!!
	  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/

/*

	TODO LIST : 

	* 1. ObjectType type -> conteaza doar la parsare => sterge din proprietatile unui SceneObject

	* 2. rezolva rotate camera

	  -----------------------------------------------------------------------------------------------------------------------------

	* 3. Forward Declarations in toate fisierele

	* 4. rotate camera - mouse input

	* 5. ObjectType type -> conteaza doar la parsare => sterge din proprietatile unui SceneObject

	* 6. CONTROLS -> MERGI IN SUS/JOS GLOBAL (NU RELATIV CUM ESTE ACM)
				  -> MERGI IN INAINTE/INAPO GLOBAL (NU RELATIV CUM ESTE ACM)

	* 7. pt fiecare resource -> sa fie data in Init()

	* 8. pt ceata - fara if => clamp

	* 9. verifica fiecare pointer inainte de a-l folosi

	* 10. 

*/

