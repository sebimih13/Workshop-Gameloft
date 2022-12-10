#include "stdafx.h"
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Camera.h"

SceneObject::SceneObject()
{

}

SceneObject::SceneObject(const SceneObject& obj) :	ID(obj.ID),
													position(obj.position), rotation(obj.rotation), scale(obj.scale),
													modelID(obj.modelID), model(obj.model),
													shaderID(obj.shaderID), shader(obj.shader),
													textureIDs(obj.textureIDs), textures(obj.textures),
													name(obj.name), wiredFormat(obj.wiredFormat)
													
{
	
}

void SceneObject::Load()
{
	model = ResourceManager::getInstance()->LoadModel(modelID);
	shader = ResourceManager::getInstance()->LoadShader(shaderID);

	for (int& id : textureIDs)
	{
		Texture* texture = ResourceManager::getInstance()->LoadTexture(id);
		textures.push_back(texture);
	}
}

void SceneObject::Draw()
{
	// Bind
	glUseProgram(shader->getProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, model->getVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getEBO());

	// TODO : choose color / textures
	if (textures.size() > 0)
	{
		// TODO : loop for multiple textures
		glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureID());
	}
	else
	{
		// TODO : set color
		shader->setColor(&color);
	}

	// TODO : matrix model -> sa NU fie calculat la fiecare draw ddaca este obiect static
	// TODO : matrix model = scale * rotation * position
	Matrix translationMatrix;
	translationMatrix.SetTranslation(position);

	Matrix rotationMatrix;
	Matrix rotMatrix;
	rotationMatrix.SetIdentity();	
	rotationMatrix = rotationMatrix * rotMatrix.SetRotationX(rotation.x);
	rotationMatrix = rotationMatrix * rotMatrix.SetRotationY(rotation.y);
	rotationMatrix = rotationMatrix * rotMatrix.SetRotationZ(rotation.z);

	Matrix scaleMatrix;
	scaleMatrix.SetScale(scale);

	// TODO : check camera pointer

	Matrix modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	Matrix mvp = modelMatrix * camera->getViewMatrix() * camera->getProjectionMatrix();

	// TODO : check
	shader->setPosition();
	shader->setUV();
	shader->setMVP(&mvp);

	// Draw
	glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_INT, 0);

	// Unbind
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO : choose color / textures
	if (textures.size() > 0)
	{
		// TODO : put in a loop
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneObject::debug()
{
	std::cout << "\t ID : " << ID << '\n';

	std::cout << "\t position : " << position << '\n';
	std::cout << "\t rotation : " << rotation << '\n';
	std::cout << "\t scale : " << scale << '\n';

	std::cout << "\t modelID : " << modelID << '\n';
	std::cout << "\t shaderID : " << shaderID << '\n';

	std::cout << "\t TEXTURES : " << textureIDs.size() << '\n';
	int textIndex = 1;
	for (auto& texture : textureIDs)
	{
		std::cout << "\t\t texture " << textIndex++ << " : " << texture << '\n';
	}

	std::cout << "\t name : " << name << '\n';
	std::cout << "\t type : " << type << '\n';
	std::cout << "\t wiredFormat : " << wiredFormat << '\n';

	std::cout << '\n';
}

