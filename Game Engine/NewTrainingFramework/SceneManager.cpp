#include "stdafx.h"
#include "SceneManager.h"

#include "../Utilities/NodeXML.hpp"
#include "../Utilities/AttributeXML.hpp"

#include "TerrainObject.h"
#include "SkyboxObject.h"
#include "FireObject.h"

#include "FogEffect.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

	// Fog Effect
	NodeXML fogNode = rootNode.getChild("fog");
	if (fogNode.isValid())
	{
		fogEffect = new FogEffect();

		fogEffect->setr(fogNode.getChild("r").getFloat());
		fogEffect->setR(fogNode.getChild("R").getFloat());

		Vector3 color;
		color.x = fogNode.getChild("color").getChild("r").getFloat();
		color.y = fogNode.getChild("color").getChild("g").getFloat();
		color.z = fogNode.getChild("color").getChild("b").getFloat();
		fogEffect->setColor(color);
	}

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

	// Lights
	NodeXML lightsNode = rootNode.getChild("lights");
	for (NodeXML lightNode = lightsNode.getChild("light"); lightNode.isValid(); lightNode = lightNode.getNextSibling())
	{
		int id = lightNode.getAttribute("id").getInt();

		Vector3 ambientColor;
		ambientColor.x = lightNode.getChild("ambientColor").getChild("r").getFloat();
		ambientColor.y = lightNode.getChild("ambientColor").getChild("g").getFloat();
		ambientColor.z = lightNode.getChild("ambientColor").getChild("b").getFloat();
		
		Vector3 diffuseColor;
		diffuseColor.x = lightNode.getChild("diffuseColor").getChild("r").getFloat();
		diffuseColor.y = lightNode.getChild("diffuseColor").getChild("g").getFloat();
		diffuseColor.z = lightNode.getChild("diffuseColor").getChild("b").getFloat();

		Vector3 specularColor;
		specularColor.x = lightNode.getChild("specularColor").getChild("r").getFloat();
		specularColor.y = lightNode.getChild("specularColor").getChild("g").getFloat();
		specularColor.z = lightNode.getChild("specularColor").getChild("b").getFloat();

		// Create new light
		Light* light = nullptr;
		LightType lightType = getLightType(lightNode.getChild("type").getString());

		switch (lightType)
		{
			case LightType::Directional:
			{
				Vector3 direction;
				direction.x = lightNode.getChild("direction").getChild("x").getFloat();
				direction.y = lightNode.getChild("direction").getChild("y").getFloat();
				direction.z = lightNode.getChild("direction").getChild("z").getFloat();

				light = new DirectionalLight(lightType, ambientColor, diffuseColor, specularColor, direction);
			}
			break;

			case LightType::Point:
			{
				Vector3 position;
				position.x = lightNode.getChild("position").getChild("x").getFloat();
				position.y = lightNode.getChild("position").getChild("y").getFloat();
				position.z = lightNode.getChild("position").getChild("z").getFloat();

				float constant = lightNode.getChild("constant").getFloat();
				float linear = lightNode.getChild("linear").getFloat();
				float quadratic = lightNode.getChild("quadratic").getFloat();

				light = new PointLight(lightType, ambientColor, diffuseColor, specularColor, position, constant, linear, quadratic);
			}
			break;

			case LightType::Spotlight:
			{
				Vector3 position;
				position.x = lightNode.getChild("position").getChild("x").getFloat();
				position.y = lightNode.getChild("position").getChild("y").getFloat();
				position.z = lightNode.getChild("position").getChild("z").getFloat();

				Vector3 direction;
				direction.x = lightNode.getChild("direction").getChild("x").getFloat();
				direction.y = lightNode.getChild("direction").getChild("y").getFloat();
				direction.z = lightNode.getChild("direction").getChild("z").getFloat();

				float constant = lightNode.getChild("constant").getFloat();
				float linear = lightNode.getChild("linear").getFloat();
				float quadratic = lightNode.getChild("quadratic").getFloat();

				float cutOff = lightNode.getChild("cutOff").getFloat();
				float outerCutOff = lightNode.getChild("outerCutOff").getFloat();

				// input in radians
				cutOff = cosf(getRadians(cutOff));
				outerCutOff = cosf(getRadians(outerCutOff));

				light = new SpotLight(lightType, ambientColor, diffuseColor, specularColor, position, direction, constant, linear, quadratic, cutOff, outerCutOff);
			}
			break;
		}

		// Adauga lumina
		lights.insert({ id, light });


		/*
			TODO : pt fiecare light -> creeaza un obiect care sa reprezinta aceasta lumina 
				   -> sa nu mai fie un obiect separat in XML
		*/

		int pointLightIndex = 0;
		int spotLightIndex = 0;

		if (lightType != LightType::Directional)
		{
			SceneObject* lightObj = new SceneObject();

			lightObj->setType(ObjectType::Normal);
			lightObj->setCamera(cameras[activeCameraID]);

			lightObj->setID(100 + pointLightIndex + spotLightIndex);

			// TODO : function to create light object
			Vector3 position;
			position.x = lightNode.getChild("position").getChild("x").getFloat();
			position.y = lightNode.getChild("position").getChild("y").getFloat();
			position.z = lightNode.getChild("position").getChild("z").getFloat();
			lightObj->setPosition(position);

			Vector3 rotation;
			rotation.x = 0.0f;
			rotation.y = 0.0f;
			rotation.z = 0.0f;
			lightObj->setRotation(rotation);

			lightObj->setShader(11);

			lightObj->setColor(diffuseColor);	// TODO : ambient / diffuse / specular 

			// Set Effects		TODO : rewrite this class
			lightObj->setFog(fogEffect);

			if (lightType == LightType::Point)
			{
				lightObj->setName("LightPoint_" + std::to_string(pointLightIndex++));

				Vector3 scale;
				scale.x = 0.3f;
				scale.y = 0.3f;
				scale.z = 0.3f;
				lightObj->setScale(scale);

				lightObj->setModel(5);
			}
			else if (lightType == LightType::Spotlight)
			{
				lightObj->setName("SpotPoint_" + std::to_string(spotLightIndex++));

				Vector3 scale;
				scale.x = 5.0f;
				scale.y = 5.0f;
				scale.z = 5.0f;
				lightObj->setScale(scale);

				lightObj->setModel(6);
			}

			// Adauga obiectul
			objects.push_back(lightObj);
		}
	}

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
					skyboxObj->setModel(objectNode.getChild("model").getInt());
				}
				break;

			case ObjectType::Fire:
				{
					FireObject* fireObj = new FireObject();
					obj = fireObj;

					// seteaza atributile speciale (care se regasesc doar in clasa asta)
					fireObj->setModel(objectNode.getChild("model").getInt());
					fireObj->setDispMax(objectNode.getChild("dispMax").getFloat());
				}
				break;
		}

		/* TODO : modifica structura la XML
			* pt model generated - sa nu mai punem <model>...</model> 
			pt fiecare obiect verificam daca are model
			altfel inseamna ca e generat
		*/

		obj->setType(objType);
		obj->setCamera(cameras[activeCameraID]);

		obj->setName(objectNode.getChild("name").getString());
		obj->setID(objectNode.getAttribute("id").getInt());

		// Set Effects
		obj->setFog(fogEffect);

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
		rotation.x = getRadians(objectNode.getChild("rotation").getChild("x").getFloat());
		rotation.y = getRadians(objectNode.getChild("rotation").getChild("y").getFloat());
		rotation.z = getRadians(objectNode.getChild("rotation").getChild("z").getFloat());
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

		// following camera
		NodeXML followingCameraNode = objectNode.getChild("followingCamera");
		if (followingCameraNode.isValid())
		{
			Vector3 axis;

			if (followingCameraNode.getChild("ox").isValid())
				axis.x = 1.0f;
			if (followingCameraNode.getChild("oy").isValid())
				axis.y = 1.0f;
			if (followingCameraNode.getChild("oz").isValid())
				axis.z = 1.0f;

			obj->setFollowingCamera(axis);
		}

		// lights
		NodeXML objLightsNode = objectNode.getChild("lights");
		for (NodeXML lightNode = objLightsNode.getChild("light"); lightNode.isValid(); lightNode = lightNode.getNextSibling())
		{
			int lightID = lightNode.getInt();
			if (lights[lightID])
			{
				obj->addLight(lights[lightID]);
			}
		}

		objects.push_back(obj);
	}

	// TODO : debugSettings

	// TODO : debug class
	debugClass();
}

void SceneManager::Clear()
{
	// TODO : delete stuffs
	delete fogEffect;
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

	for (SceneObject* object : objects)
	{
		object->Update();

		// TODO : 

		// daca am schimbat camera activa => object->setCamera(cameras[activeCameraID]);
		// altfel o setez o singura data in Init

		// daca schimbam position, rotation, scale	=> object->setPosition(...)
		//											=> object->setRotation(...)
		//											=> object->setScale(...)
		//											=> updateModelMatrix
		// altfel calculez o singura data modelMatrix
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
	else if (type == "fire")
		return ObjectType::Fire;
	return ObjectType::ObjectType_DEFAULT;

	// TODO : ADD MORE
}

LightType SceneManager::getLightType(std::string& type)
{
	if (type == "directional")
		return LightType::Directional;
	else if (type == "point")
		return LightType::Point;
	else if (type == "spotlight")
		return LightType::Spotlight;
	return LightType::LightType_DEFAULT;

	// TODO : ADD MORE
}

void SceneManager::debugClass()
{
	std::cout << "\n\n\n\n\tSCENE MANAGER\n";

	std::cout << "gameName : " << gameName << '\n';
	std::cout << "width : " << defaultScreenSize.width << '\n';
	std::cout << "height : " << defaultScreenSize.height << '\n';
	std::cout << "backgroundColor : " << backgroundColor.x << ' ' << backgroundColor.y << ' ' << backgroundColor.z << '\n';

	// fog
	fogEffect->debug();
	
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

	// lights
	std::cout << "\n  Lights : " << lights.size() << "\n\n";
}

