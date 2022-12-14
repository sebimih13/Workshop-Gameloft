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
	Terrain,
	Skybox
};

class SceneObject
{
public:
	/** Constructor */
	SceneObject();

	/** Load model + shader + textures */
	virtual void Load();

	/** Draw Object */
	virtual void Draw();

	/** Update Object */
	virtual void Update();

	/** Debug */
	void debug();

	/** Getters / Setters */
	inline void setType(ObjectType t) { type = t; }
	inline void setCamera(Camera* cam) { camera = cam; }

	inline void setID(int id) { ID = id; }
	inline void setName(std::string _name) { name = _name; }
	inline void setWiredFormat(bool wired) { wiredFormat = wired; }

	inline void setPosition(Vector3 _position) { position = _position; }
	inline void setRotation(Vector3 _rotation) { rotation = _rotation; }
	inline void setScale(Vector3 _scale) { scale = _scale; }

	inline void setModel(int id) { modelID = id; }
	inline void setShader(int id) { shaderID = id; };
	inline void setTextures(std::vector<int> ids) { textureIDs = ids; }
	inline void setColor(Vector3 col) { color = col; }

	inline void setFollowingCamera(Vector3 axis) { followingCamera = axis; }

protected:
	ObjectType type;		// TODO : teoretic nu mai e nevoie de type aici (este nevoie de el doar cand parsam XML-ul)
	Camera* camera;

	/** Attributes */
	std::string name;
	bool wiredFormat;

	int ID;		// TODO : make unsigned int

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

private:
	/** Color */		// TODO : O LAS AICI?
	Vector3 color;

	/** Following camera */
	Vector3 followingCamera;
	
	/** Others */
	// TODO : bool depthTest;
};

// TODO : INITIALIZE STRUCTS + CLASS IN CONSTRUCTOR ? 

