#include "stdafx.h"
#include "SceneObject.h"

#include "ResourceManager.h"
#include "SceneManager.h"

#include "Camera.h"
#include "Light.h"
#include "CollisionComponent.h"

#include "Axes.h"

SceneObject::SceneObject()
{
	type = ObjectType::Normal;
	camera = nullptr;

	name = "DEFAULT_NAME";
	wiredFormat = false;

	drawCollision = false;
	activeCollision = false;

	drawAxes = false;

	trajectory = nullptr;

	collision = nullptr;

	// TODO : adaug restul?
}

SceneObject::~SceneObject()
{
	delete collision;
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

	// Effects
	if (fogEffect && shader && camera)
	{
		fogEffect->LoadUniforms(shader);
		fogEffect->setCamera(camera);
	}

	// Set camera offsets
	followingCameraOffset.x = position.x - camera->getPosition().x;
	followingCameraOffset.y = position.y - camera->getPosition().y;
	followingCameraOffset.z = position.z - camera->getPosition().z;

	// Collision
	if (model)
	{
		LoadCollision(model->getVertices());
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

	// Set Attributes
	shader->setPosition();
	shader->setNormal();
	shader->setBinormal();
	shader->setTangent();
	shader->setUV();

	// Set Uniforms

	// TODO : modelMatrix + camera -> trb setate intr-un update doar cand se schimba
	shader->setMVP(&mvp);
	shader->setModelMatrixUniform(&modelMatrix);
	shader->setColor(&color);

	// Set Effects
	fogEffect->SetUniforms();

	// Set camera position for lights
	shader->setCameraViewPosition(&camera->getPosition());

	// Set lights
	int directionalLightIndex = 0;
	int pointLightIndex = 0;
	int spotLightIndex = 0;

	for (Light* light : lights)
	{
		switch (light->getType())
		{
			case LightType::Directional:	light->LoadUniforms(shader, directionalLightIndex++);	break;
			case LightType::Point:			light->LoadUniforms(shader, pointLightIndex++);			break;
			case LightType::Spotlight:		light->LoadUniforms(shader, spotLightIndex++);			break;
		}
		light->SetUniforms();
	}

	// Draw Object
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
		glActiveTexture(GL_TEXTURE0 + i);	// TODO : asta mai trb pusa?
		glBindTexture(textures[i]->getTextureType(), 0);
	}

	// TODO : Draw collision
	if (drawCollision && getCollisionComponent())
	{
		// Get Collision Shader
		Shader* collisionShader = ResourceManager::getInstance()->LoadShader(16);

		// Load Collision Render Data
		glUseProgram(collisionShader->getProgramID());
		glBindBuffer(GL_ARRAY_BUFFER, getCollisionComponent()->getVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getCollisionComponent()->getWiredEBO());

		// Set Attributes
		collisionShader->setPosition();

		// Set Uniforms
		collisionShader->setMVP(&mvp);
		collisionShader->setColor(&Vector3(1.0f, 0.0f, 0.0f));		// TODO : choose color from XML

		// Draw Collision
		glDrawElements(GL_LINES, getCollisionComponent()->getNrIndicesWired(), GL_UNSIGNED_INT, 0);

		// Unbind
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (drawAxes)
	{
		scaleMatrix.SetScale(100.0f);
		modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

		// TODO : check camera pointer
		Matrix mvpAxes = modelMatrix * camera->getViewMatrix() * camera->getProjectionMatrix();

		SceneManager::getInstance()->getAxes()->Draw(&mvpAxes);
	}
}

void SceneObject::Update(float deltaTime)
{
	// TODO : calculate modelMatrix for collision and draw
	// here

	// Update collision
	if (activeCollision)	// TODO
	{
		calculateCollision();
	}

	// Update position
	if (trajectory)
	{
		trajectory->applyTrajectory(position, rotation, deltaTime);
	}
	
	// Following Objects
	if (followingCamera.x != 0.0f)
	{
		position.x = camera->getPosition().x + followingCameraOffset.x;
	}

	if (followingCamera.y != 0.0f)
	{
		position.y = camera->getPosition().y + followingCameraOffset.y;
	}

	if (followingCamera.z != 0.0f)
	{
		position.z = camera->getPosition().z + followingCameraOffset.z;
	}
}

void SceneObject::LoadCollision(std::vector<Vertex>& verticesData)
{
	float minX = verticesData[0].pos.x;
	float maxX = verticesData[0].pos.x;

	float minY = verticesData[0].pos.y;
	float maxY = verticesData[0].pos.y;

	float minZ = verticesData[0].pos.z;
	float maxZ = verticesData[0].pos.z;

	for (Vertex& v : verticesData)
	{
		minX = min(minX, v.pos.x);
		maxX = max(maxX, v.pos.x);

		minY = min(minY, v.pos.y);
		maxY = max(maxY, v.pos.y);

		minZ = min(minZ, v.pos.z);
		maxZ = max(maxZ, v.pos.z);
	}

	collision = new CollisionComponent(minX, maxX, minY, maxY, minZ, maxZ);
}

void SceneObject::calculateCollision()
{
	// TODO : Calcualte collision box's vertices

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

	Matrix modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	getCollisionComponent()->CalculateVertexPosition(modelMatrix);
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

