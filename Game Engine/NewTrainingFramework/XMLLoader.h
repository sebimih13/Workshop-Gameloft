#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

#include "../Utilities/Math.h"

using namespace rapidxml;

enum ControlsConfig
{
	DEFAULT,					// ERROR

	MOVE_CAMERA_POSITIVE_Z,		// W
	MOVE_CAMERA_NEGATIVE_Z,		// S

	MOVE_CAMERA_POSITIVE_X,		// A
	MOVE_CAMERA_NEGATIVE_X,		// D

	MOVE_CAMERA_POSITIVE_Y,		// Q
	MOVE_CAMERA_NEGATIVE_Y		// E
};

struct ScreenConfig
{
	unsigned int width;
	unsigned int height;
};

struct CameraConfig
{
	enum CameraType
	{
		none,

		firstPerson,
		thirdPerson
	} type;

	Vector3 position;
	Vector3 target;
	Vector3 up;
	float translationSpeed;
	float rotationSpeed;
	float fov;
	float nearPlane;
	float farPlane;
};

struct Object
{
	/** Constructor */
	Object() {  }

	/** Copy Constructor */
	Object(const Object& obj) : ID(obj.ID), model(obj.model), shader(obj.shader), 
								type(obj.type), 
								name(obj.name), textures(obj.textures),
								position(obj.position), rotation(obj.rotation), scale(obj.scale)
	{  }

	int ID;
	int model;
	int shader;
	std::string name;

	enum ObjectType
	{
		normal,
		terrain
	} type;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	std::vector<int> textures;		// TODO : textures / color
	// TODO : bool wired;
};

class XMLLoaderSceneManager
{
public:
	/** Constructor */
	XMLLoaderSceneManager(char* path);

	/** Destructor */
	~XMLLoaderSceneManager();

private:
	/** Data */
	std::string gameName;
	ScreenConfig defaultScreenSize;
	Vector3 backgroundColor;
	std::map<char, ControlsConfig> controls;
	
	int activeCameraID;
	std::map<int, CameraConfig> cameras;

	std::vector<Object> objects;

	/** Utilities Functions */
	ControlsConfig getControlsAction(std::string action);
	CameraConfig::CameraType getCameraType(std::string type);

	// TODO : DEBUG
	void debugClass();
};


// TODO : INITIALIZE STRUCTS => CONSTRUCTOR
// TODO : change name to SceneManager

