#include "stdafx.h"
#include "XMLLoader.h"

#include "../Utilities/rapidxml-1.13/rapidxml_print.hpp"
#include "../Utilities/NodeXML.hpp"
#include "../Utilities/AttributeXML.hpp"

/*
	--------------------------------------------------------------------------------------------------------------------------------
													XML LOADER FOR SCENE MANAGER
	--------------------------------------------------------------------------------------------------------------------------------
*/

XMLLoaderSceneManager::XMLLoaderSceneManager(char* path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());

	xml_document<> doc;
	doc.parse<0>(&content[0]);

	// Root Node
	xml_node<>* pNodeRoot = doc.first_node();
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

		CameraConfig config;

		config.type = getCameraType(cameraNode.getChild("type").getString());
		
		config.position.x = cameraNode.getChild("position").getChild("x").getFloat();
		config.position.y = cameraNode.getChild("position").getChild("y").getFloat();
		config.position.z = cameraNode.getChild("position").getChild("z").getFloat();

		config.target.x = cameraNode.getChild("target").getChild("x").getFloat();
		config.target.y = cameraNode.getChild("target").getChild("y").getFloat();
		config.target.z = cameraNode.getChild("target").getChild("z").getFloat();

		config.up.x = cameraNode.getChild("up").getChild("x").getFloat();
		config.up.y = cameraNode.getChild("up").getChild("y").getFloat();
		config.up.z = cameraNode.getChild("up").getChild("z").getFloat();

		config.translationSpeed = cameraNode.getChild("translationSpeed").getFloat();
		config.rotationSpeed	= cameraNode.getChild("rotationSpeed").getFloat();
		config.fov				= cameraNode.getChild("fov").getFloat();

		config.nearPlane		= cameraNode.getChild("near").getFloat();
		config.farPlane			= cameraNode.getChild("far").getFloat();

		cameras.insert({ id, config });
	}

	// Active Camera
	activeCameraID = rootNode.getChild("activeCamera").getInt();

	// Objects
	NodeXML objectsNode = rootNode.getChild("objects");
	for (NodeXML objectNode = objectsNode.getChild("object"); objectNode.isValid(); objectNode = objectNode.getNextSibling())
	{
		Object obj;

		obj.ID = objectNode.getAttribute("id").getInt();
		obj.model = 13;		// TODO : model (ce reprezinta ???)
		obj.shader = objectNode.getChild("shader").getInt();
		obj.type = Object::ObjectType::normal;	// TODO : type
		obj.name = objectNode.getChild("name").getString();

		// textures
		NodeXML texturesNode = objectNode.getChild("textures");
		for (NodeXML textureNode = texturesNode.getChild("texture"); textureNode.isValid(); textureNode = textureNode.getNextSibling())
		{
			int textureID = textureNode.getAttribute("id").getInt();
			obj.textures.push_back(textureID);
		}

		obj.position.x = objectNode.getChild("position").getChild("x").getFloat();
		obj.position.y = objectNode.getChild("position").getChild("y").getFloat();
		obj.position.z = objectNode.getChild("position").getChild("z").getFloat();

		obj.rotation.x = objectNode.getChild("rotation").getChild("x").getFloat();
		obj.rotation.y = objectNode.getChild("rotation").getChild("y").getFloat();
		obj.rotation.z = objectNode.getChild("rotation").getChild("z").getFloat();

		obj.scale.x = objectNode.getChild("scale").getChild("x").getFloat();
		obj.scale.y = objectNode.getChild("scale").getChild("y").getFloat();
		obj.scale.z = objectNode.getChild("scale").getChild("z").getFloat();

		objects.push_back(obj);
	}

	debugClass();
}

/** Destructor */
XMLLoaderSceneManager::~XMLLoaderSceneManager()
{

}

ControlsConfig XMLLoaderSceneManager::getControlsAction(std::string action)
{
	if (action == "MOVE_CAMERA_POSITIVE_Z")					// W
		return ControlsConfig::MOVE_CAMERA_POSITIVE_Z;
	else if (action == "MOVE_CAMERA_NEGATIVE_Z")			// S
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_Z;
	else if (action == "MOVE_CAMERA_POSITIVE_X")			// A
		return ControlsConfig::MOVE_CAMERA_POSITIVE_X;
	else if (action == "MOVE_CAMERA_NEGATIVE_X")			// D
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_X;
	else if (action == "MOVE_CAMERA_POSITIVE_Y")			// Q
		return ControlsConfig::MOVE_CAMERA_POSITIVE_Y;
	else if (action == "MOVE_CAMERA_NEGATIVE_Y")			// E
		return ControlsConfig::MOVE_CAMERA_NEGATIVE_Y;
	return ControlsConfig::DEFAULT;						// ERROR

	// TODO : ADD MORE
}

CameraConfig::CameraType XMLLoaderSceneManager::getCameraType(std::string type)
{
	if (type == "firstPerson")
		return CameraConfig::CameraType::firstPerson;
	else if (type == "thirdPerson")
		return CameraConfig::CameraType::thirdPerson;
	return CameraConfig::CameraType::none;
}

void XMLLoaderSceneManager::debugClass()
{
	std::cout << "gameName : " << gameName << '\n';
	std::cout << "width : " << defaultScreenSize.width << '\n';
	std::cout << "height : " << defaultScreenSize.height << '\n';
	std::cout << "backgroundColor : " << backgroundColor.x << ' ' << backgroundColor.y << ' ' << backgroundColor.z << '\n';
	
	// controls
	for (auto itr = controls.begin(); itr != controls.end(); ++itr)
	{
		std::cout << itr->first << '\t' << itr->second << '\n';
	}

	// cameras
	for (auto itr = cameras.begin(); itr != cameras.end(); ++itr)
	{
		std::cout << itr->first << " : ";

		std::cout << '\t' << "type : " << itr->second.type << '\n';

		std::cout << '\t' << "position : " << itr->second.position << '\n';
		std::cout << '\t' << "target : " << itr->second.target << '\n';
		std::cout << '\t' << "up : " << itr->second.up << '\n'; 

		std::cout << '\t' << "translationSpeed : " << itr->second.translationSpeed << '\n';
		std::cout << '\t' << "rotationSpeed : " << itr->second.rotationSpeed << '\n';
		std::cout << '\t' << "fov : " << itr->second.fov << '\n';

		std::cout << '\t' << "nearPlane : " << itr->second.nearPlane << '\n';
		std::cout << '\t' << "farPlane : " << itr->second.farPlane << '\n';
	}

	// active camera
	std::cout << "active camera : " << activeCameraID << '\n';

	// objects
	int index = 1;
	for (auto& object : objects)
	{
		std::cout << index++ << " : \n";
		std::cout << "\t ID : " << object.ID << '\n';
		std::cout << "\t model : " << object.model << '\n';
		std::cout << "\t shader : " << object.shader << '\n';
		std::cout << "\t type : " << object.type << '\n';
		std::cout << "\t name : " << object.name << '\n';

		// TODO : textures
		int textIndex = 1;
		for (auto& texture : object.textures)
		{
			std::cout << "\t texture " << textIndex++  << " : " << texture << '\n';
		}

		std::cout << "\t position : " << object.position << '\n';
		std::cout << "\t rotation : " << object.rotation << '\n';
		std::cout << "\t scale : " << object.scale << '\n';
	}
}

