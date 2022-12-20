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
	void SetUniforms(Matrix* model);		// TODO : sa pastrez parametrul asta?

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
	GLuint modelMatrixUniform;

	/** Set uniforms functions */
	void setFogrUniform();
	void setFogRUniform();
	void setFogColorUniform();

	void setFogCameraPosUniform();
	void setModelMatrixUniform(Matrix* model);
};

