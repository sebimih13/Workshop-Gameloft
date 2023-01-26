#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

/** Forward Declarations */
class Shader;

enum LightType
{
	LightType_DEFAULT,

	Point,
	Directional,
	Spotlight
};

class Light
{
public:
	/** Constructor */
	Light();

	/** Getters / Setters */
	inline void setPosition(Vector3 pos) { position = pos; }
	inline void setDiffuseColor(Vector3 color) { diffuseColor = color; }
	inline void setSpecularColor(Vector3 color) { specularColor = color; }

	// TODO : private
	int ID;

	Vector3 position;
	Vector3 diffuseColor;		// TODO : use in shader (make uniform)
	Vector3 specularColor;		// TODO : use in shader (make uniform)

	LightType type;				// TODO : aici ?
};

