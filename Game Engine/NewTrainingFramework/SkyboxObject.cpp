#include "stdafx.h"
#include "SkyboxObject.h"

#include "Camera.h"

SkyboxObject::SkyboxObject()
{

}

SkyboxObject::~SkyboxObject()
{

}

void SkyboxObject::Load()
{
	position = camera->getPosition();

	// call parent method to load model + shader + textures
	SceneObject::Load();
}

void SkyboxObject::Draw()
{
	// call parent method
	SceneObject::Draw();
}

void SkyboxObject::Update()
{
	// TODO
}

