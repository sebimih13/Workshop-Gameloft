#include "stdafx.h"
#include "SceneManager.h"

#include "../Utilities/rapidxml-1.13/rapidxml_print.hpp"
#include "../Utilities/NodeXML.hpp"
#include "../Utilities/AttributeXML.hpp"

#include "TerrainObject.h"
#include "SkyboxObject.h"

// Instantiate static variables
SceneManager* SceneManager::instance = nullptr;

void SceneManager::Init(char* filePath)
{
	std::ifstream file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());

	xml_document<> doc;
	doc.parse<0>(&content[0]);

	// Root Node
	NodeXML rootNode(doc.first_node());

	// Game name
	gameName = rootNode.getChild("gameName").getString();

	// Default Screen Size
	defaultScreenSize.width = rootNode.getChild("defaultScreenSize").getChild("width").getInt();
	defaultScreenSize.height = rootNode.getChild("defaultScreenSize").getChild("height").getInt();

	// Background Color
	backgroundColor.x = rootNode.getChild("backgroundColor").getChild("r").getFloat();
	backgroundColor.y = rootNode.getChild("backgroundColor").getChild("g").getFloat();
	backgroundColor.z = rootNode.getChild("backgroundColor").getChild("b").getFloat();

	// Controls
	NodeXML controlsNode = rootNode.getChild("controls");
	for (NodeXML controlNode = controlsNode.getChild("control"); controlNode.isValid(); controlNode = controlNode.getNextSibling())
	{
		char key = controlNode.getChild("key").getString()[0];
		std::string action = controlNode.getChild("action").getString();

		controls.insert({ key, getControlsAction(action) });
	}

	// Cameras
	NodeXML camerasNode = rootNode.getChild("cameras");
	for (NodeXML cameraNode = camerasNode.getChild("camera"); cameraNode.isValid(); cameraNode = cameraNode.getNextSibling())
	{
		int id = cameraNode.getAttribute("id").getInt();

		// Set camera properties manually
		Camera* camera = new Camera();

		camera->setType(getCameraType(cameraNode.getChild("type").getString()));

		Vector3 position;
		position.x = cameraNode.getChild("position").getChild("x").getFloat();
		position.y = cameraNode.getChild("position").getChild("y").getFloat();
		position.z = cameraNode.getChild("position").getChild("z").getFloat();
		camera->setPosition(position);

		Vector3 target;
		target.x = cameraNode.getChild("target").getChild("x").getFloat();
		target.y = cameraNode.getChild("target").getChild("y").getFloat();
		target.z = cameraNode.getChild("target").getChild("z").getFloat();
		camera->setTarget(target);

		Vector3 up;
		up.x = cameraNode.getChild("up").getChild("x").getFloat();
		up.y = cameraNode.getChild("up").getChild("y").getFloat();
		up.z = cameraNode.getChild("up").getChild("z").getFloat();
		camera->setUp(up);

		camera->setTranslationSpeed(cameraNode.getChild("translationSpeed").getFloat());
		camera->setRotationSpeed(cameraNode.getChild("rotationSpeed").getFloat());
		camera->setFOV(cameraNode.getChild("fov").getFloat());

		camera->setNearPlane(cameraNode.getChild("near").getFloat());
		camera->setFarPlane(cameraNode.getChild("far").getFloat());

		// TODO : Init camera manually
		camera->Init();

		cameras.insert({ id, camera });
	}

	// Active Camera
	activeCameraID = rootNode.getChild("activeCamera").getInt();

	// Objects
	NodeXML objectsNode = rootNode.getChild("objects");
	for (NodeXML objectNode = objectsNode.getChild("object"); objectNode.isValid(); objectNode = objectNode.getNextSibling())
	{
		SceneObject* obj = nullptr;

		// make different objects based on type
		ObjectType objType = getObjectType(objectNode.getChild("type").getString());

		switch (objType)
		{
			case ObjectType::Normal:
				{
					obj = new SceneObject();

					// seteaza atributile speciale (care se regasesc doar in clasa asta)
					obj->setModel(objectNode.getChild("model").getInt());
				}
				break;

			case ObjectType::Terrain:
				{
					TerrainObject* terrainObj = new TerrainObject();
					obj = terrainObj;
				
					// seteaza atributile speciale (care se regasesc doar in clasa asta)
					terrainObj->setModel(-1);

					Vector3 height;
					height.x = objectNode.getChild("height").getChild("r").getFloat();
					height.y = objectNode.getChild("height").getChild("g").getFloat();
					height.z = objectNode.getChild("height").getChild("b").getFloat();
					terrainObj->setHeight(height);
				}
				break;

			case ObjectType::Skybox:
				{
					SkyboxObject* skyboxObj = new SkyboxObject();
					obj = skyboxObj;

					// seteaza atributile speciale (care se regasesc doar in clasa asta)
					obj->setModel(objectNode.getChild("model").getInt());
				}
				break;
		}

		obj->setType(objType);
		obj->setCamera(cameras[activeCameraID]);

		obj->setName(objectNode.getChild("name").getString());
		obj->setID(objectNode.getAttribute("id").getInt());

		NodeXML wiredNode = objectNode.getChild("wired");
		if (wiredNode.isValid())
		{
			obj->setWiredFormat(true);
		}
		else
		{
			obj->setWiredFormat(false);
		}

		Vector3 position;
		position.x = objectNode.getChild("position").getChild("x").getFloat();
		position.y = objectNode.getChild("position").getChild("y").getFloat();
		position.z = objectNode.getChild("position").getChild("z").getFloat();
		obj->setPosition(position);

		Vector3 rotation;
		rotation.x = objectNode.getChild("rotation").getChild("x").getFloat();
		rotation.y = objectNode.getChild("rotation").getChild("y").getFloat();
		rotation.z = objectNode.getChild("rotation").getChild("z").getFloat();
		obj->setRotation(rotation);

		Vector3 scale;
		scale.x = objectNode.getChild("scale").getChild("x").getFloat();
		scale.y = objectNode.getChild("scale").getChild("y").getFloat();
		scale.z = objectNode.getChild("scale").getChild("z").getFloat();
		obj->setScale(scale);

		// textures
		std::vector<int> texturesIDs;
		NodeXML texturesNode = objectNode.getChild("textures");
		for (NodeXML textureNode = texturesNode.getChild("texture"); textureNode.isValid(); textureNode = textureNode.getNextSibling())
		{
			int textureID = textureNode.getAttribute("id").getInt();
			texturesIDs.push_back(textureID);
		}

		obj->setShader(objectNode.getChild("shader").getInt());
		obj->setTextures(texturesIDs);

		// color
		NodeXML colorNode = objectNode.getChild("color");
		if (colorNode.isValid())
		{
			Vector3 color;
			color.x = colorNode.getChild("r").getFloat();
			color.y = colorNode.getChild("g").getFloat();
			color.z = colorNode.getChild("b").getFloat();

			obj->setColor(color);
		}

		objects.push_back(obj);
	}

	// TODO : debugSettings

	debugClass();
}

void SceneManager::Clear()
{
	// TODO : delete stuffs
}

SceneManager* SceneManager::getInstance()
{
	if (!instance)
	{
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::LoadObjects()
{
	// Load objects
	for (SceneObject* object : objects)
	{
		object->Load();
	}
}

void SceneManager::Draw()
{
	// Draw objects
	for (SceneObject* object : objects)
	{
		object->Draw();
	}
}

void SceneManager::Update()
{
	// TODO : inca nu stiu

	// daca am schimbat camera activa => object->setCamera(cameras[activeCameraID]);
	// altfel o setez o singura data in LoadObjects

	for (SceneObject* object : objects)
	{
		object->Update();
	}
}

ControlsConfig SceneManager::getControlsAction(std::string& action)
{
	if (action == "MOVE_CAMERA_POSITIVE_X")					// A
		return ControlsConfig::MOVE_CAMERA_POSITIVE_X;
	else if (action == "MOVE_CAMERA_NEGATIVE_X")			// D
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_X;

	else if (action == "MOVE_CAMERA_POSITIVE_Y")			// Q
		return ControlsConfig::MOVE_CAMERA_POSITIVE_Y;
	else if (action == "MOVE_CAMERA_NEGATIVE_Y")			// E
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_Y;

	else if (action == "MOVE_CAMERA_POSITIVE_Z")			// W
		return ControlsConfig::MOVE_CAMERA_POSITIVE_Z;
	else if (action == "MOVE_CAMERA_NEGATIVE_Z")			// S
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_Z;

	if (action == "ROTATE_CAMERA_POSITIVE_X")				// ARROW UP
		return ControlsConfig::ROTATE_CAMERA_POSITIVE_X;
	else if (action == "ROTATE_CAMERA_NEGATIVE_X")			// ARROW DOWN
		return ControlsConfig::ROTATE_CAMERA_NEGATIVE_X;

	if (action == "ROTATE_CAMERA_POSITIVE_Y")				// ARROW LEFT
		return ControlsConfig::ROTATE_CAMERA_POSITIVE_Y;
	else if (action == "ROTATE_CAMERA_NEGATIVE_Y")			// ARROW RIGHT
		return ControlsConfig::ROTATE_CAMERA_NEGATIVE_Y;

	if (action == "ROTATE_CAMERA_POSITIVE_Z")				// O
		return ControlsConfig::ROTATE_CAMERA_POSITIVE_Z;
	else if (action == "ROTATE_CAMERA_NEGATIVE_Z")			// P
		return ControlsConfig::ROTATE_CAMERA_NEGATIVE_Z;

	return ControlsConfig::DEFAULT_CONTROLSCONFIG;			// ERROR

	// TODO : ADD MORE
}

CameraType SceneManager::getCameraType(std::string& type)
{
	if (type == "firstPerson")
		return CameraType::firstPerson;
	else if (type == "thirdPerson")
		return CameraType::thirdPerson;
	return CameraType::none;

	// TODO : ADD MORE
}

ObjectType SceneManager::getObjectType(std::string& type)
{
	if (type == "normal")
		return ObjectType::Normal;
	else if (type == "terrain")
		return ObjectType::Terrain;
	else if (type == "skybox")
		return ObjectType::Skybox;
	return ObjectType::DEFAULT_TYPE;

	// TODO : ADD MORE
}

void SceneManager::debugClass()
{
	std::cout << "\n\n\n\n\tSCENE MANAGER\n";

	std::cout << "gameName : " << gameName << '\n';
	std::cout << "width : " << defaultScreenSize.width << '\n';
	std::cout << "height : " << defaultScreenSize.height << '\n';
	std::cout << "backgroundColor : " << backgroundColor.x << ' ' << backgroundColor.y << ' ' << backgroundColor.z << '\n';
	
	// controls
	std::cout << "\nCONTROLS : \n";
	for (auto& itr = controls.begin(); itr != controls.end(); ++itr)
	{
		std::cout << '\t' << itr->first << '\t' << itr->second << '\n';
	}

	// cameras
	std::cout << "\nCAMERAS : \n";
	for (auto& itr = cameras.begin(); itr != cameras.end(); ++itr)
	{
		std::cout << itr->first << " : ";

		std::cout << '\t' << "type : " << itr->second->getType() << '\n';

		std::cout << '\t' << "position : " << itr->second->getPosition() << '\n';
		std::cout << '\t' << "target : " << itr->second->getTarget() << '\n';
		std::cout << '\t' << "up : " << itr->second->getUp() << '\n';

		std::cout << '\t' << "translationSpeed : " << itr->second->getTranslationSpeed() << '\n';
		std::cout << '\t' << "rotationSpeed : " << itr->second->getRotationSpeed() << '\n';
		std::cout << '\t' << "fov : " << itr->second->getFOV() << '\n';

		std::cout << '\t' << "nearPlane : " << itr->second->getNearPlane() << '\n';
		std::cout << '\t' << "farPlane : " << itr->second->getFarPlane() << '\n';
	}

	// active camera
	std::cout << "active camera : " << activeCameraID << '\n';

	// objects
	std::cout << "\nOBJECTS : " << '\n';
	int index = 1;
	for (SceneObject* object : objects)
	{
		object->debug();
	}
}

