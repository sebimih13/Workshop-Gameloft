#pragma once

#include "../Utilities/esUtil.h"
#include "../Utilities/Math.h"

enum CameraType
{
	none,

	firstPerson,
	thirdPerson
};

class Camera
{
public:
	/** Constructors */
	Camera();

	/** Destructors */
	~Camera();

	/** Init */
	void Init();

	/** Move functions */
	void moveOx(GLfloat value);
	void moveOy(GLfloat value);
	void moveOz(GLfloat value);

	/** Rotate functions */
	void rotateOx(GLfloat value);
	void rotateOy(GLfloat value);
	void rotateOz(GLfloat value);

	/** Update functions */
	void updateWorldView();
	void updateAxis();

	/** Getters / Setters */
	inline void setDeltaTime(GLfloat delta) { deltaTime = delta; }
	inline Matrix getViewMatrix() { return viewMatrix; }					// TODO : Matrix&
	inline Matrix getProjectionMatrix() { return projectionMatrix; }		// TODO : Matrix&

	inline void setPosition(Vector3 _position) { position = _position; }
	inline Vector3& getPosition() { return position; }

	inline void setTarget(Vector3 _target) { target = _target; }
	inline Vector3& getTarget() { return target; }

	inline void setUp(Vector3 _up) { up = _up; }
	inline Vector3& getUp() { return up; }

	inline void setTranslationSpeed(float speed) { translationSpeed = speed; }
	inline float& getTranslationSpeed() { return translationSpeed; }

	inline void setRotationSpeed(float speed) { rotationSpeed = speed; }
	inline float& getRotationSpeed() { return rotationSpeed; }

	inline void setFOV(float value) { fov = value; }
	inline float& getFOV() { return fov; }

	inline void setNearPlane(float value) { nearPlane = value; }
	inline float& getNearPlane() { return nearPlane; }

	inline void setFarPlane(float value) { farPlane = value; }
	inline float& getFarPlane() { return farPlane; }

	inline void setType(CameraType typeToSet) { type = typeToSet; }
	inline CameraType& getType() { return type; }

private:
	CameraType type;

	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	GLfloat translationSpeed;
	GLfloat rotationSpeed;

	GLfloat fov;
	GLfloat nearPlane;
	GLfloat farPlane;

	GLfloat deltaTime;

	Matrix projectionMatrix, viewMatrix, worldMatrix;
};

