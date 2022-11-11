#include "stdafx.h"		// TODO : first include file

#include "Camera.h"

Camera::Camera(Vector3 _position, Vector3 _target, Vector3 _up = Vector3(0.0f, 1.0f, 0.0f))
{
	position = _position;
	target = _target;
	up = _up;

	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	moveSpeed = 0.0f;
	rotateSpeed = 0.0f;

	nearPlane = 0.0f;
	farPlane = 0.0f;
	fov = 0.0f;

	deltaTime = 0.0f;

	viewMatrix = Matrix();
	worldMatrix = Matrix();
}

Camera::~Camera()
{
	
}

void Camera::moveOx(GLfloat value)
{
	Matrix direction;
	direction.SetIdentity();
	direction.m[0][0] = value;

	Vector3 forward = -(target - position).Normalize(); // TODO :  *(xAxis * value);
	Vector3 deplasare = forward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;
}

void Camera::moveOy(GLfloat value)
{

}

void Camera::moveOz(GLfloat value)
{

}

void Camera::rotateOx()
{

}

void Camera::rotateOy()
{

}

void Camera::rotateOz()
{

}

void Camera::updateWorldView()
{
	Matrix R;
	R.m[0][0] = xAxis.x;	R.m[0][1] = xAxis.y;	R.m[0][2] = xAxis.z;	R.m[0][3] = 0.0f;
	R.m[1][0] = yAxis.x;	R.m[1][1] = yAxis.y;	R.m[1][2] = yAxis.z;	R.m[1][3] = 0.0f;
	R.m[2][0] = zAxis.x;	R.m[2][1] = zAxis.y;	R.m[2][2] = zAxis.z;	R.m[2][3] = 0.0f;
	R.m[3][0] = 0.0f;		R.m[3][1] = 0.0f;		R.m[3][2] = 0.0f;		R.m[3][3] = 1.0f;

	Matrix T;
	T.SetIdentity();
	T.m[3][0] = position.x;		T.m[3][1] = position.y;		T.m[3][2] = position.z;

	worldMatrix = R * T;

	T.m[3][0] = -position.x;		T.m[3][1] = -position.y;		T.m[3][2] = -position.z;
	viewMatrix = T * R.Transpose();
}

