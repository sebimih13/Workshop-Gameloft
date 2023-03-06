#include "stdafx.h"
#include "Mirror.h"

#include "SceneManager.h"
#include "ResourceManager.h"

Mirror::Mirror()
{
	shader = nullptr;
}

Mirror::~Mirror()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteRenderbuffers(1, &RBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &texture);
}

void Mirror::Load()
{
	unsigned int& screenWidth = SceneManager::getInstance()->getDefaultScreenSize().width;
	unsigned int& screenHeight = SceneManager::getInstance()->getDefaultScreenSize().height;

	// initialize FBO
	glGenFramebuffers(1, &FBO);

	// initialize RBO
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenWidth, screenHeight);	// TODO ?: DEPTH24 + STENCIL8

	// intialize texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// configure FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// check for framebuffer complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR : Failed to initialize Framebuffer\n";
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// configure VBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/**
		TODO : calculeaza automat in functie de ScreenSize

		width  = 960		=> 0.60 = -0.3 -> 0.3
		height = 720		=> 0.45 = 0.55 -> 1.0

	*/

	std::vector<Vertex> verticesData = {
		Vertex(Vector3(-0.3f, 0.55f, 0.0f), Vector2(0.0f, 0.0f)),	// 0 - stanga jos
		Vertex(Vector3(-0.3f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)),	// 1 - stanga sus
		Vertex(Vector3(0.3f,  0.55f, 0.0f), Vector2(1.0f, 0.0f)),	// 2 - dreapta jos
		Vertex(Vector3(0.3f,  1.0f, 0.0f), Vector2(1.0f, 1.0f))		// 3 - dreapta sus
	};

	std::vector<unsigned int> indicesData = {
		// triunghi sus
		0, 3, 1,

		// triunghi jos
		0, 2, 3
	};

	nrIndices = indicesData.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &indicesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Load post processing shader
	shader = ResourceManager::getInstance()->LoadShader(17);
}

void Mirror::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClearColor(SceneManager::getInstance()->getBackgroundColor().x,
		SceneManager::getInstance()->getBackgroundColor().y,
		SceneManager::getInstance()->getBackgroundColor().z,
		1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Mirror::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// TODO : delete from NewTrainingFramework Draw()
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Mirror::Draw()
{
	glUseProgram(shader->getProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDisable(GL_DEPTH_TEST);

	// Set Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setTexture(0);

	// Set Attributes
	shader->setPosition();
	shader->setUV();

	// Set uniforms
	// TODO

	// Draw
	glDrawElements(GL_TRIANGLES, nrIndices, GL_UNSIGNED_INT, 0);

	// Unbind
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

