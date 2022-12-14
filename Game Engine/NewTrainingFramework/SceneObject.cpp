#include "stdafx.h"
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Camera.h"

SceneObject::SceneObject()
{
	type = ObjectType::Normal;
	camera = nullptr;

	name = "DEFAULT_NAME";
	wiredFormat = false;

	// TODO : adaug restul?
}

void SceneObject::Load()
{
	if (modelID != -1)
		model = ResourceManager::getInstance()->LoadModel(modelID);
	if (shaderID != -1)
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

	// Set textures
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(textures[i]->getTextureType(), textures[i]->getTextureID());
		shader->setTexture(i);

	}

	// TODO : matrix model -> sa NU fie calculat la fiecare Draw() ddaca este obiect static
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
	shader->setColor(&color);

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

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// TODO : pt unbind e bine?
		glActiveTexture(GL_TEXTURE0 + i);	// TODO : asta mai trb pusa
		glBindTexture(textures[i]->getTextureType(), 0);
	}
}

void SceneObject::Update()
{
	if (followingCamera.x != 0.0f)
	{
		static const float offsetX = position.x - camera->getPosition().x;
		position.x = camera->getPosition().x + offsetX;
	}

	if (followingCamera.y != 0.0f)
	{
		static const float offsetY = position.y - camera->getPosition().y;
		position.y = camera->getPosition().y + offsetY;
	}

	if (followingCamera.z != 0.0f)
	{
		static const float offsetZ = position.z - camera->getPosition().z;
		position.z = camera->getPosition().z + offsetZ;
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

