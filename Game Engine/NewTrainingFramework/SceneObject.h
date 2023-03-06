#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

#include "FogEffect.h"

/** Forward Declarations */
class Camera;
class Light;
class Trajectory;
class CollisionComponent;

enum ObjectType
{
	ObjectType_DEFAULT,

	Normal,
	Terrain,
	Skybox,
	Fire
};

class SceneObject
{
public:
	/** Constructor */
	SceneObject();

	/** Destructor */
	~SceneObject();

	/** Load model + shader + textures */
	virtual void Load();

	/** Draw Object */
	virtual void Draw();

	/** Update Object */
	virtual void Update(float deltaTime);

	/** Collision Box */
	void LoadCollision(std::vector<Vertex>& verticesData);
	void calculateCollision();

	/** Debug */
	void debug();

	/** Getters / Setters */

	/** Setters */
	inline void setType(ObjectType t) { type = t; }
	inline void setCamera(Camera* cam) { camera = cam; }

	inline void setID(int id) { ID = id; }
	inline void setName(std::string _name) { name = _name; }

	inline void setWiredFormat(bool wired) { wiredFormat = wired; }

	inline void setDrawCollision(bool enable) { drawCollision = enable; }

	inline void setActiveCollision(bool enable) { activeCollision = enable; }
	inline bool getActiveCollision() { return activeCollision; }

	inline void setDrawAxes(bool enable) { drawAxes = enable; }

	inline void setPosition(Vector3 _position) { position = _position; }
	inline void setRotation(Vector3 _rotation) { rotation = _rotation; }
	inline void setScale(Vector3 _scale) { scale = _scale; }

	inline void setModel(int id) { modelID = id; }
	inline void setShader(int id) { shaderID = id; };
	inline void setTextures(std::vector<int> ids) { textureIDs = ids; }
	inline void setColor(Vector3 col) { color = col; }

	inline void setFollowingCamera(Vector3 axis) { followingCamera = axis; }

	/* Set Effects */
	inline void setFog(FogEffect* fog) { fogEffect = new FogEffect(fog); }

	/* Set Ambiental Light */
	inline void setAmbientalLightColor(Vector3* color) { ambientalLightColor = color; }
	inline void setAmbientalLightStrength(float strength) { ambientalLightStrength = strength; }

	/** Lights */
	inline void addLight(Light* light) { lights.push_back(light); }

	/** Trajectory */
	inline void setTrajectory(Trajectory* traj) { trajectory = traj; }

	/** Getters */
	inline std::string& getName() { return name; }

	CollisionComponent* getCollisionComponent() { return collision; }

protected:
	ObjectType type;		// TODO : teoretic nu mai e nevoie de type aici (este nevoie de el doar cand parsam XML-ul)
	Camera* camera;

	/** Attributes */
	std::string name;
	bool wiredFormat;

	/** Collision */
	bool drawCollision;
	bool activeCollision;

	/** Localspace Axes */
	bool drawAxes;

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

	/** Ambiental Light */
	Vector3* ambientalLightColor;
	float ambientalLightStrength;

	/** Lights */
	std::vector<Light*> lights;

	/** Following camera */
	Vector3 followingCamera;
	Vector3 followingCameraOffset;

	/** Fog Effect */
	FogEffect* fogEffect;

	/** Trajectory */
	Trajectory* trajectory;

	/** Collision */
	CollisionComponent* collision;
	 
	/** Others */
	// TODO : bool depthTest;
};

// TODO : INITIALIZE STRUCTS + CLASS IN CONSTRUCTOR ? 

