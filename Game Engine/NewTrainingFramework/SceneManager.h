#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

#include "Camera.h"
#include "SceneObject.h"

using namespace rapidxml;

enum ControlsConfig
{
	DEFAULT,					// None

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
	ROTATE_CAMERA_NEGATIVE_Z	// P
};

struct ScreenConfig
{
	unsigned int width;
	unsigned int height;
};

class SceneManager
{
public:
	void Init(char* filePath);
	void Clear();

	static SceneManager* getInstance();

	/** Load objects */
	void LoadObjects();

	/** Draw Objects */
	void Draw();

	/** Upadate objects */
	void Update();

	/** Getters */
	inline std::string& getGameName() { return gameName; }
	inline ScreenConfig& getDefaultScreenSize() { return defaultScreenSize; }
	inline Vector3& getBackgroundColor() { return backgroundColor; }
	inline std::map<char, ControlsConfig>& getControls() { return controls; }

	inline Camera* getActiveCamera() { return cameras[activeCameraID]; }

private:
	/** Constructor */
	SceneManager() {  };
	 
	/** Instance */
	static SceneManager* instance;

	/** Debug */
	void debugClass();

	/** Data */
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

	/** Utilities Functions */
	ControlsConfig getControlsAction(std::string& action);
	CameraType getCameraType(std::string& type);
};


// TODO : INITIALIZE STRUCTS + CLASS IN CONSTRUCTOR ? 
// TODO : add const + static

