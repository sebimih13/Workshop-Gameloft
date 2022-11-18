#pragma once

// TODO : ? Forward Declarations sau #include ... ?

#include "../Utilities/esUtil.h"
#include "../Utilities/Math.h"

class Camera
{
public:
	/** Constructors */
	Camera(Vector3 _position, Vector3 _target, Vector3 _up = Vector3(0.0f, 1.0f, 0.0f));

	/** Destructors */
	~Camera();

	/** Move functions */
	void moveOx(GLfloat value);
	void moveOy(GLfloat value);
	void moveOz(GLfloat value);

	/** Rotate functions */
	void rotateOx();
	void rotateOy();
	void rotateOz();

	/** Update functions */
	void updateWorldView();
	void updateAxis();

	/** Getters / Setters */
	inline void setDeltaTime(GLfloat delta) { deltaTime = delta; }
	inline Matrix getViewMatrix() { return viewMatrix; }
	inline Matrix getProjectionMatrix() { return projectionMatrix; }

private:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	GLfloat moveSpeed;
	GLfloat rotateSpeed;

	GLfloat nearPlane;
	GLfloat farPlane;
	GLfloat fov;

	GLfloat deltaTime;

	Matrix projectionMatrix, viewMatrix, worldMatrix;
};

