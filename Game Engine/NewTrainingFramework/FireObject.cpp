#include "stdafx.h"
#include "FireObject.h"

FireObject::FireObject()
{
	time = 0.0f;
}

FireObject::~FireObject()
{

}

void FireObject::Load()
{
	// call parent method to load : model + shader + textures
	SceneObject::Load();

	// Load Uniforms
	timeUniform = glGetUniformLocation(shader->getProgramID(), "u_time");
	dispMaxUniform = glGetUniformLocation(shader->getProgramID(), "u_dispMax");
}

void FireObject::Draw()
{
	glUseProgram(shader->getProgramID());

	// Set uniforms
	setTime();
	setDispMax();

	// call parent method
	SceneObject::Draw();
}

void FireObject::Update(float deltaTime)
{
	// TODO : calculeaza pe baza clock()
	time += 0.007f; 
	if (time > 1.0f)
	{
		time = 0.0f;
	}

	// call parent method
	SceneObject::Update(deltaTime);
}

void FireObject::setTime()
{
	if (timeUniform != -1)
	{
		glUniform1f(timeUniform, time);
	}
}

void FireObject::setDispMax()
{
	if (dispMaxUniform != -1)
	{
		glUniform1f(dispMaxUniform, dispMax);
	}
}

