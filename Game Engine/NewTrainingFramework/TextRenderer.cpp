#include "stdafx.h"
#include "TextRenderer.h"

#include "ResourceManager.h"

// Instantiate static variables
TextRenderer* TextRenderer::instance = nullptr;

TextRenderer::TextRenderer()
{
	// Generate buffer
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
}

TextRenderer::~TextRenderer()
{
	glDeleteBuffers(1, &VBO);
}

TextRenderer* TextRenderer::getInstance()
{
	if (!instance)
	{
		instance = new TextRenderer();
	}
	return instance;
}

void TextRenderer::Draw(std::string text, int fontID, float x, float y, float scale, Vector3 color)
{
	//Font* font = ResourceManager::getInstance()->LoadFont(fontID);
	//Shader* shader = ResourceManager::getInstance()->LoadShader(18);

	//glUseProgram(shader->getProgramID());
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//for (char& c : text)
	//{
	//	Character ch = font->getCharacter(c);

	//	float xpos = x + ch.Bearing.x * scale;
	//	float ypos = y + (font->getCharacter('H').Bearing.y - ch.Bearing.y) * scale;

	//	float w = ch.Size.x * scale;
	//	float h = ch.Size.y * scale;

	//	// update VBO
	//	std::vector<Vertex> verticesData = {
	//		Vertex(Vector3(xpos,		ypos + h,	0.0f), Vector2(0.0f, 1.0f)),	
	//		Vertex(Vector3(xpos + w,	ypos,		0.0f), Vector2(1.0f, 0.0f)),	
	//		Vertex(Vector3(xpos,		ypos,		0.0f), Vector2(0.0f, 0.0f)),	

	//		Vertex(Vector3(xpos,		ypos + h,	0.0f), Vector2(0.0f, 1.0f)),
	//		Vertex(Vector3(xpos + w,	ypos + h,	0.0f), Vector2(1.0f, 1.0f)),
	//		Vertex(Vector3(xpos + w,	ypos,		0.0f), Vector2(1.0f, 0.0f))
	//	};

	//	// render glyph texture over quad
	//	glBindTexture(GL_TEXTURE_2D, ch.TextureID);

	//	// update content of VBO memory
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesData.size() * sizeof(Vertex), &verticesData[0]);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//	// Set Attributes
	//	shader->setPosition();
	//	shader->setUV();

	//	// Set Uniforms
	//	shader->setColor(&color);

	//	// render quad
	//	glDrawArrays(GL_TRIANGLES, 0, 6);

	//	// advence cursors for next glyph
	//	x += (ch.Advance >> 6) * scale;
	//}

	//// Unbind
	//glUseProgram(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

