#include "stdafx.h"
#include "SceneObject.h"
#include "ResourceManager.h"

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

	// TODO : put in a loop
	glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureID());

	// TODO : check
	shader->setPosition();
	shader->setUV();
	shader->setMVP(mvp);

	// Draw
	glDrawElements(GL_TRIANGLES, model->getNrIndices(), GL_UNSIGNED_INT, 0);

	// Unbind
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO : put in a loop
	glBindTexture(GL_TEXTURE_2D, 0);
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
	std::cout << "\t wiredFormat : " << wiredFormat << '\n';

	std::cout << '\n';
}

