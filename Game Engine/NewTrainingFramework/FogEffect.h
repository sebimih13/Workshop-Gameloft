#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

/** Forward Declarations */
class Shader;
class Camera;

class FogEffect
{
public:
	/** Constructor */
	FogEffect();
	FogEffect(FogEffect* fog);

	/** Destructor */
	~FogEffect();

	/** Load */
	void LoadUniforms(Shader* shader);

	/** Set Uniforms */
	void SetUniforms();		// TODO : sa pastrez parametrul asta?

	/** Debug */
	void debug();

	/** Getters / Setters */
	inline void setr(float value) { r = value; }
	inline void setR(float value) { R = value; }
	inline void setColor(Vector3 col) { color = col; }

	inline void setCamera(Camera* cam) { camera = cam; }
	// TODO : la fel si pt model

private:
	/** Proprietati */
	float r;
	float R;
	Vector3 color;

	/** Variabile de setat */
	Camera* camera;

	/** Uniforms */
	GLuint fogrUniform;
	GLuint fogRUniform;
	GLuint fogColorUniform;

	GLuint fogCameraPosUniform;

	/** Set uniforms functions */
	void setFogrUniform();
	void setFogRUniform();
	void setFogColorUniform();

	void setFogCameraPosUniform();
};

/*
	TODO : REFACTOR THIS CLASS
	
	sa fie asemanatoare cu clasa Light
	sa retina doar proprietatile
	si restul obiectelor au un pointer catre clasa FogEffect creata in SceneManager.cpp 

	move uniforms to SceneObject.h
*/

