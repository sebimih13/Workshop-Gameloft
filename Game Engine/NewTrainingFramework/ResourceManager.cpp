#include "stdafx.h"
#include "ResourceManager.h"

#include "../Utilities/NodeXML.hpp"
#include "../Utilities/AttributeXML.hpp"

// Instantiate static variables
ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::Init(char* filePath)
{
	// Initialize Sound System
	if (FMOD::System_Create(&fmodSystem) != FMOD_OK)
	{
		std::cout << "ERROR : SOUND SYSTEM NOT CREATED !!! \n";
	}
	else
	{
		std::cout << "SOUND SYSTEM CREATED !!! \n";
		fmodSystem->init(36, FMOD_INIT_NORMAL, NULL);
	}

	// Parse XML file
	std::ifstream file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());

	xml_document<> doc;
	doc.parse<0>(&content[0]);

	// Root Node
	NodeXML rootNode(doc.first_node());

	// Models Node
	NodeXML modelsNode = rootNode.getChild("models");
	for (NodeXML folderNode = modelsNode.getChild("folder"); folderNode.isValid(); folderNode = folderNode.getNextSibling())
	{
		for (NodeXML modelNode = folderNode.getChild("model"); modelNode.isValid(); modelNode = modelNode.getNextSibling())
		{
			ModelResource* resource = new ModelResource();
			resource->filePath = folderNode.getAttribute("path").getString();
			resource->id = modelNode.getAttribute("id").getInt();
			resource->fileName = modelNode.getChild("file").getString();

			modelResources.insert({ resource->id, resource });
		}
	}

	// Textures Node
	NodeXML texturesNode = rootNode.getChild("textures");
	for (NodeXML folderNode = texturesNode.getChild("folder"); folderNode.isValid(); folderNode = folderNode.getNextSibling())
	{
		for (NodeXML textureNode = folderNode.getChild("texture"); textureNode.isValid(); textureNode = textureNode.getNextSibling())
		{
			TextureResource* resource = new TextureResource();
			
			resource->id = textureNode.getAttribute("id").getInt();
			resource->type = getTextureType(textureNode.getAttribute("type").getString());
			
			resource->filePath = folderNode.getAttribute("path").getString();
			resource->fileName = textureNode.getChild("file").getString();

			resource->minFilter = getTextureFilter(textureNode.getChild("min_filter").getString());
			resource->magFilter = getTextureFilter(textureNode.getChild("mag_filter").getString());

			resource->wrapS = getTextureWrapMode(textureNode.getChild("wrap_s").getString());
			resource->wrapT = getTextureWrapMode(textureNode.getChild("wrap_t").getString());

			textureResources.insert({ resource->id, resource });
		}
	}

	// Shaders Node
	NodeXML shadersNode = rootNode.getChild("shaders");
	for (NodeXML folderNode = shadersNode.getChild("folder"); folderNode.isValid(); folderNode = folderNode.getNextSibling())
	{
		for (NodeXML shaderNode = folderNode.getChild("shader"); shaderNode.isValid(); shaderNode = shaderNode.getNextSibling())
		{
			ShaderResource* resource = new ShaderResource();

			resource->filePath = folderNode.getAttribute("path").getString();
			resource->id = shaderNode.getAttribute("id").getInt();

			resource->fileVS = shaderNode.getChild("vs").getString();
			resource->fileFS = shaderNode.getChild("fs").getString();

			shaderResources.insert({ resource->id, resource });
		}
	}

	// Sounds Node
	NodeXML soundsNode = rootNode.getChild("sounds");
	for (NodeXML folderNode = soundsNode.getChild("folder"); folderNode.isValid(); folderNode = folderNode.getNextSibling())
	{
		for (NodeXML soundNode = folderNode.getChild("sound"); soundNode.isValid(); soundNode = soundNode.getNextSibling())
		{
			SoundResource* resource = new SoundResource();

			std::string folder = folderNode.getAttribute("path").getString();
			std::string fileName = soundNode.getChild("file").getString();

			resource->filePath = folder + fileName;
			resource->id = soundNode.getAttribute("id").getInt();

			soundResources.insert({ resource->id, resource });
		}
	}

	debug();
}

void ResourceManager::Clear()
{
	// TODO : delete stuffs
}

ResourceManager* ResourceManager::getInstance()
{
	if (!instance)
	{
		instance = new ResourceManager();
	}
	return instance;
}

Model* ResourceManager::LoadModel(int id)
{
	if (models[id] == nullptr)
	{
		models[id] = new Model();
		models[id]->Load(modelResources[id]);
	}
	return models[id];
}

Texture* ResourceManager::LoadTexture(int id)
{
	if (textures[id] == nullptr)
	{
		textures[id] = new Texture(textureResources[id]);
		textures[id]->Load();
	}
	return textures[id];
}

Shader* ResourceManager::LoadShader(int id)
{
	if (shaders[id] == nullptr)
	{
		shaders[id] = new Shader(shaderResources[id]);
		shaders[id]->Load();
	}
	return shaders[id];
}

Sound* ResourceManager::LoadSound(int id)
{
	if (sounds[id] == nullptr)
	{
		sounds[id] = new Sound(soundResources[id]);
		sounds[id]->Load(fmodSystem);
	}
	return sounds[id];
}

TextureType ResourceManager::getTextureType(std::string type)
{
	if (type == "2d")
		return TextureType::TEXTURE_2D;
	else if (type == "cube")
		return TextureType::TEXTURE_CUBE;
	return TextureType::DEFAULT_TEXTURETYPE;

	// TODO : add more
}

TextureFilter ResourceManager::getTextureFilter(std::string filter)
{
	if (filter == "LINEAR")
		return TextureFilter::LINEAR;
	else if (filter == "NEAREST")
		return TextureFilter::NEAREST;
	return TextureFilter::DEFAULT_TEXTUREFILTER;
}

TextureWrapMode ResourceManager::getTextureWrapMode(std::string mode)
{
	if (mode == "CLAMP_TO_EDGE")
		return TextureWrapMode::CLAMP_TO_EDGE;
	else if (mode == "REPEAT")
		return TextureWrapMode::REPEAT;
	else if (mode == "MIRRORED_REPEAT")
		return TextureWrapMode::MIRRORED_REPEAT;
	return DEFAULT_TEXTUREWRAPMODE;
}

void ResourceManager::debug()
{
	std::cout << "\n\n\n\n\tRESOURCE MANAGER\n";

	// MODELS
	std::cout << "\nMODELS : " << modelResources.size() << '\n';
	for (auto& itr = modelResources.begin(); itr != modelResources.end(); ++itr)
	{
		std::cout << "MODEL " << itr->first << " : ";
		std::cout << "Path : " << itr->second->filePath << '\n';
		std::cout << "\t  Name : " << itr->second->fileName << '\n';
	}

	// Textures
	std::cout << "\nTEXTURES : " << textureResources.size() << '\n';
	for (auto& itr = textureResources.begin(); itr != textureResources.end(); ++itr)
	{
		std::cout << "TEXTURE " << itr->first << " : ";
		std::cout << "Path : " << itr->second->filePath << '\n';
		std::cout << "\t  Name : " << itr->second->fileName << '\n';
	}

	// Shaders
	std::cout << "\nSHADERS : " << shaderResources.size() << '\n';
	for (auto& itr = shaderResources.begin(); itr != shaderResources.end(); ++itr)
	{
		std::cout << "SHADER " << itr->first << " : ";
		std::cout << "Path : " << itr->second->filePath << '\n';
		std::cout << "\t   VS : " << itr->second->fileVS << '\n';
		std::cout << "\t   FS : " << itr->second->fileFS << '\n';
	}

	std::cout << "\n\n";
}

