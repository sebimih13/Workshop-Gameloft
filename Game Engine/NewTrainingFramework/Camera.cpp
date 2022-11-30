#include "stdafx.h"	
#include "Camera.h"

#include "SceneManager.h"

Camera::Camera()
{
	type = CameraType::firstPerson;

	position = Vector3(0.0f, 0.0f, -100.0f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);

	translationSpeed = 100.0f;
	rotationSpeed = 1.0f;

	nearPlane = 0.01f;
	farPlane = 10000.0f;
	fov = 45.0f;

	deltaTime = 0.0f;

	Init();
}

Camera::~Camera()
{
	
}

void Camera::Init()
{
	float radiansFOV = fov * PI / 180.0f;
	float aspect = float(SceneManager::getInstance()->getDefaultScreenSize().width) / float(SceneManager::getInstance()->getDefaultScreenSize().height);

	projectionMatrix = Matrix().SetPerspective(radiansFOV, aspect, nearPlane, farPlane);
	updateAxis();
	updateWorldView();
}

void Camera::moveOx(GLfloat value)
{
	Vector3 left = xAxis * value;
	Vector3 deplasare = left * translationSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::moveOy(GLfloat value)
{
	Vector3 upward = yAxis * value;
	Vector3 deplasare = upward * translationSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::moveOz(GLfloat value)
{
	Vector3 forward = zAxis * value;
	Vector3 deplasare = forward * translationSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::rotateOx(GLfloat value)
{
	Matrix mRotateOX;
	mRotateOX.SetRotationX(rotationSpeed * deltaTime * value);

	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	Vector4 up4 = rotatedLocalUp * worldMatrix;
	up = Vector3(up4.x, up4.y, up4.z).Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target = Vector3(target4.x, target4.y, target4.z);

	updateAxis();
	updateWorldView();
}

void Camera::rotateOy(GLfloat value)
{
	Matrix mRotateOY;
	mRotateOY.SetRotationY(rotationSpeed * deltaTime * value);

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);	
	Vector4 rotatedTarget = localTarget * mRotateOY;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target = Vector3(target4.x, target4.y, target4.z);

	updateAxis();
	updateWorldView();
}

void Camera::rotateOz(GLfloat value)
{
	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(rotationSpeed * deltaTime * value);

	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	Vector4 up4 = rotatedLocalUp * worldMatrix;
	up = Vector3(up4.x, up4.y, up4.z).Normalize();

	updateAxis();
	updateWorldView();
}

void Camera::updateAxis()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updateWorldView()
{
	Matrix R;
	R.m[0][0] = xAxis.x;	R.m[0][1] = xAxis.y;	R.m[0][2] = xAxis.z;	R.m[0][3] = 0.0f;
	R.m[1][0] = yAxis.x;	R.m[1][1] = yAxis.y;	R.m[1][2] = yAxis.z;	R.m[1][3] = 0.0f;
	R.m[2][0] = zAxis.x;	R.m[2][1] = zAxis.y;	R.m[2][2] = zAxis.z;	R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f;		R.m[3][1] = 0.0f;		R.m[3][2] = 0.0f;		R.m[3][3] = 1.0f;

	Matrix T;
	T.SetTranslation(position);

	worldMatrix = R * T;

	T.SetTranslation(-position.x, -position.y, -position.z);
	viewMatrix = T * R.Transpose();
}

