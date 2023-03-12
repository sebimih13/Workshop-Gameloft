#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Sound.h"
#include "Font.h"

#include "fmod.hpp"

class ResourceManager
{
public:
	void Init(char* filePath);
	void Clear();

	/** Get Instance */
	static ResourceManager* getInstance();

	Model* LoadModel(int id);
	Texture* LoadTexture(int id);
	Shader* LoadShader(int id);
	Sound* LoadSound(int id);
	Font* LoadFont(int id);

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

	/** Shaders */
	std::map<int, ShaderResource*> shaderResources;
	std::map<int, Shader*> shaders;

	/** Sounds */
	std::map<int, SoundResource*> soundResources;
	std::map<int, Sound*> sounds;

	/** Fonts */
	std::map<int, FontResource*> fontResources;
	std::map<int, Font*> fonts;
	 
	/** FMOD System */
	FMOD::System* fmodSystem;

	/** Utilities Functions */
	TextureType getTextureType(std::string type);
	TextureFilter getTextureFilter(std::string filter);
	TextureWrapMode getTextureWrapMode(std::string mode);

	/** Debug */
	void debug();
};

// TODO : add const + static

