#pragma once

#include "stdafx.h"
#include "../Utilities/Math.h"

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

	inline void setDiffuseColor(Vector3 color) { diffuseColor = color; }
	inline void setSpecularColor(Vector3 color) { specularColor = color; }

private:
	int ID;
	Vector3 diffuseColor;
	Vector3 specularColor;

	LightType type;
};

