#include "stdafx.h"
#include "ResourceManager.h"

#include "../Utilities/rapidxml-1.13/rapidxml_print.hpp"
#include "../Utilities/NodeXML.hpp"
#include "../Utilities/AttributeXML.hpp"

// Instantiate static variables
ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::Init(char* filePath)
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
			resource->type = TextureType::TEXTURE_2D;			// TODO
			
			resource->filePath = folderNode.getAttribute("path").getString();
			resource->fileName = textureNode.getChild("file").getString();

			resource->minFilter = TextureFilter::LINEAR;		// TODO
			resource->magFilter = TextureFilter::LINEAR;		// TODO

			resource->wrapS = TextureWrapMode::CLAMP_TO_EDGE;	// TODO
			resource->wrapT = TextureWrapMode::CLAMP_TO_EDGE;	// TODO

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
		models[id] = new Model(modelResources[id]);
		models[id]->Load();
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

