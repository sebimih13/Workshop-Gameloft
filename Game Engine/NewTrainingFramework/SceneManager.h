#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"		// TODO : CIUDAT CA NU MERGE IN .CPP

#include "Camera.h"
#include "SceneObject.h"

#include "Light.h"
#include "../Utilities/Trajectories.hpp"

/** Forward Declations */
class FogEffect;
class Axes;
class PostProcessor;
class Mirror;

enum ControlsConfig
{
	DEFAULT_CONTROLSCONFIG,		// None

	MOVE_CAMERA_POSITIVE_X,		// A
	MOVE_CAMERA_NEGATIVE_X,		// D

	MOVE_CAMERA_POSITIVE_Y,		// Q
	MOVE_CAMERA_NEGATIVE_Y,		// E

	MOVE_CAMERA_POSITIVE_Z,		// W
	MOVE_CAMERA_NEGATIVE_Z,		// S

	ROTATE_CAMERA_POSITIVE_X,	// ARROW UP
	ROTATE_CAMERA_NEGATIVE_X,	// ARROW DOWN

	ROTATE_CAMERA_POSITIVE_Y,	// ARROW LEFT
	ROTATE_CAMERA_NEGATIVE_Y,	// ARROW RIGHT

	ROTATE_CAMERA_POSITIVE_Z,	// O 
	ROTATE_CAMERA_NEGATIVE_Z,	// P

	DEBUG_MODE,					// M

	GRAYSCALE_MODE,				// 1
	BLUR_MODE,					// 2
	SHARPEN_MODE				// 3

	// TODO : ADD MORE
};

struct ScreenConfig
{
	unsigned int width = 960;
	unsigned int height = 720;
};

class SceneManager
{
public:
	void Init(char* filePath);
	void Clear();

	/** Get Instance */
	static SceneManager* getInstance();

	/** Load objects */
	void Load();

	/** Draw Objects */
	void Draw();

	/** Upadate objects */
	void Update(float deltaTime);

	/** Scene Effects */
	void debugScene(bool activate);
	void grayscaleScene(bool activate);
	void blurScene(bool activate);
	void sharpenScene(bool activate);

	/** Check Collision */
	bool checkCollision(SceneObject* obj1, SceneObject* obj2);

	/** Getters */
	inline std::string& getGameName() { return gameName; }
	inline ScreenConfig& getDefaultScreenSize() { return defaultScreenSize; }
	inline Vector3& getBackgroundColor() { return backgroundColor; }
	inline std::map<char, ControlsConfig>& getControls() { return controls; }

	inline Camera* getActiveCamera() { return cameras[activeCameraID]; }

	inline Axes* getAxes() { return axes; }

private:
	/** Constructor */
	SceneManager();
	 
	/** Instance */
	static SceneManager* instance;

	/** Globals */
	std::string gameName;
	ScreenConfig defaultScreenSize;
	Vector3 backgroundColor;

	/** Controls */
	std::map<char, ControlsConfig> controls;
	
	/** Cameras */
	int activeCameraID;
	std::map<int, Camera*> cameras;

	/** Objects */
	std::vector<SceneObject*> objects;

	/** Effects */
	FogEffect* fogEffect;

	/** Lights */
	std::map<int, Light*> lights;

	/** Post processing */
	PostProcessor* postProcessor;

	/** Mirror */
	Mirror* mirror;

	/** Debug */
	bool drawSceneAxes;
	Axes* axes;

	/** Utilities Functions */
	ControlsConfig getControlsAction(std::string& action);
	CameraType getCameraType(std::string& type);
	ObjectType getObjectType(std::string& type);
	LightType getLightType(std::string& type);

	TrajectoryType getTrajectoryType(std::string& type);
	TrajectoryDirectionType getTrajectoryDirectionType(std::string& type);

	/** Debug */
	void debugClass();
};


// TODO : INITIALIZE STRUCTS + CLASS IN CONSTRUCTOR ? 
// TODO : add const + static

