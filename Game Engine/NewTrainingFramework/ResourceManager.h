#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
public:
	void Init(char* filePath);
	void Clear();

	static ResourceManager* getInstance();

	Model* LoadModel(int id);
	Texture* LoadTexture(int id);
	Shader* LoadShader(int id);

private:
	/** Constructor */
	ResourceManager() {  }

	/** Instance */
	static ResourceManager* instance;

	/** Models */
	std::map<int, ModelResource*> modelResources;
	std::map<int, Model*> models;

	/** Textures */
	std::map<int, TextureResource*> textureResources;
	std::map<int, Texture*> textures;

	/** Shader */
	std::map<int, ShaderResource*> shaderResources;
	std::map<int, Shader*> shaders;

	/** Utilities Functions */
	TextureType getTextureType(std::string type);
	TextureFilter getTextureFilter(std::string filter);
	TextureWrapMode getTextureWrapMode(std::string mode);

	/** Debug */
	void debug();
};

// TODO : add const + static

