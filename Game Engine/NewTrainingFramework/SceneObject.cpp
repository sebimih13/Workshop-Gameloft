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
	if (modelID != 0)	// TODO : CHECK	
		model = ResourceManager::getInstance()->LoadModel(modelID);
	if (shaderID != 0)	// TODO : CHECK
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

	if (!wiredFormat)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getEBO());
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getWiredEBO());
	}

	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->getTextureID());
		shader->setTexture(i);
	}
	
	if (textures.size() == 0)
	{
		// TODO : set color
		// TODO : nu mai pune if -> initializeaza color cu o valoare default in constructor
		shader->setColor(&color);
	}

	// TODO : move in TerrainObject::Draw()
	shader->setNrCelule(4);

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
	if (!wiredFormat)
	{
		glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElements(GL_LINES, model->getNrIndicesWired(), GL_UNSIGNED_INT, 0);
	}

	// Unbind
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (int i = 0; i < textures.size(); i++)
	{
		// TODO : pt unbind e bine?
		glActiveTexture(GL_TEXTURE0 + i);
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

