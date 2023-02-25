#pragma once

#include "../Utilities/utilities.h"

enum Axis
{
	Axis_DEFAULT,

	OX,
	OY,
	OZ
};

class Axes
{
public:
	/** Constructor */
	Axes(Vector3 colorOX = Vector3(1.0f, 0.0f, 0.0f), Vector3 colorOY = Vector3(0.0f, 1.0f, 0.0f), Vector3 colorOZ = Vector3(0.0f, 0.0f, 1.0f));

	/** Destructor */
	~Axes();

	/** Load */
	void Load();

	/** Draw */
	void Draw(Matrix* mvp);

private:
	/** Properties */
	Vector3 OXColor, OYColor, OZColor;

	/** Render Data */
	GLuint VBO_X, VBO_Y, VBO_Z;

	void DrawAxis(Axis axis, Matrix* mvp);
};

