#include "stdafx.h"	
#include "Camera.h"

#include "Globals.h"

Camera::Camera(Vector3 _position, Vector3 _target, Vector3 _up)
{
	position = _position;
	target = _target;
	up = _up;

	updateAxis();

	moveSpeed = 1.0f;
	rotateSpeed = 1.0f;

	nearPlane = 0.1f;
	farPlane = 10.0f;
	fov = 45.0f * PI / 180.0f;

	deltaTime = 0.0f;

	projectionMatrix = Matrix().SetPerspective(fov, (float)Globals::screenWidth / (float)Globals::screenHeight, nearPlane, farPlane);
	viewMatrix = Matrix();
	worldMatrix = Matrix();

	updateWorldView();
}

Camera::~Camera()
{
	
}

void Camera::moveOx(GLfloat value)		// value = { -1, 1 }	 (inapoi / inainte)
{
	Vector3 left = xAxis * value;
	Vector3 deplasare = left * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::moveOy(GLfloat value)
{
	Vector3 upward = yAxis * value;
	Vector3 deplasare = upward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::moveOz(GLfloat value)
{
	Vector3 forward = zAxis * value;
	Vector3 deplasare = forward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;

	updateWorldView();
}

void Camera::rotateOx()
{
	Matrix mRotateOX;
	mRotateOX.SetRotationX(rotateSpeed * deltaTime);

	Vector4 localUp = Vector4(up.x, up.y, up.z, 1.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	Vector4 up4 = rotatedLocalUp * worldMatrix;
	up = Vector3(up4.x, up4.y, up4.z);
	up.Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, zAxis.Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target = Vector3(target4.x, target4.y, target4.z);

	updateAxis();
	updateWorldView();
}

void Camera::rotateOy()
{
	Matrix mRotateOY;
	mRotateOY.SetRotationY(rotateSpeed * deltaTime);

	Vector4 localTarget = Vector4(0.0f, 0.0f, zAxis.Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOY;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target = Vector3(target4.x, target4.y, target4.z);

	updateAxis();
	updateWorldView();
}

void Camera::rotateOz()
{
	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(rotateSpeed * deltaTime);

	Vector4 localUp = Vector4(up.x, up.y, up.z, 1.0f);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	Vector4 up4 = rotatedLocalUp * worldMatrix;
	up = Vector3(up4.x, up4.y, up4.z);
	up.Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, zAxis.Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOZ;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target = Vector3(target4.x, target4.y, target4.z);

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

