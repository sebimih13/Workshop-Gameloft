#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class Camera;

enum ObjectType
{
	DEFAULT_TYPE,

	Normal,
	Terrain
};

class SceneObject
{
public:
	/** Constructor */
	SceneObject();

	/** Copy Constructor */
	SceneObject(const SceneObject& obj);

	/** Load model + shader + textures */
	void Load();

	void Draw();

	/** Debug */
	void debug();

	/** Getters / Setters */
	inline void setID(int id) { ID = id; }

	inline void setPosition(Vector3 _position) { position = _position; }
	inline void setRotation(Vector3 _rotation) { rotation = _rotation; }
	inline void setScale(Vector3 _scale) { scale = _scale; }

	inline void setName(std::string _name) { name = _name; }
	inline void setWiredFormat(bool wired) { wiredFormat = wired; }

	inline void setModel(int id) { modelID = id; }
	inline void setShader(int id) { shaderID = id; };
	inline void setTextures(std::vector<int> ids) { textureIDs = ids; }

	inline void setType(ObjectType t) { type = t; }

	// TODO : check
	inline void setCamera(Camera* cam) { camera = cam; }

private:
	int ID;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	/** Model */
	int modelID;
	Model* model;

	/** Shader */
	int shaderID;
	Shader* shader;

	/** Textures */
	std::vector<int> textureIDs;
	std::vector<Texture*> textures;

	/** Color */
	// TODO 

	/** XML Attributes */
	std::string name;
	bool wiredFormat;

	// TODO : ObjectType
	ObjectType type;
	
	/** Others */
	// TODO : bool depthTest;

	// TODO : check + add in constructos ?
	Camera* camera;	
};

// TODO : INITIALIZE STRUCTS + CLASS IN CONSTRUCTOR ? 

