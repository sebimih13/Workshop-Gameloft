#include "stdafx.h"
#include "FogEffect.h"

#include "Shader.h"
#include "Camera.h"

FogEffect::FogEffect()
{

}

FogEffect::FogEffect(FogEffect* fog)
{
	r = fog->r;
	R = fog->R;
	color = fog->color;
	camera = fog->camera;
}

FogEffect::~FogEffect()
{

}

void FogEffect::LoadUniforms(Shader* shader)
{
	fogrUniform = glGetUniformLocation(shader->getProgramID(), "u_r");
	fogRUniform = glGetUniformLocation(shader->getProgramID(), "u_R");
	fogColorUniform = glGetUniformLocation(shader->getProgramID(), "u_fogColor");
	fogCameraPosUniform = glGetUniformLocation(shader->getProgramID(), "u_cameraPos");
}

void FogEffect::SetUniforms()
{
	setFogrUniform();
	setFogRUniform();
	setFogColorUniform();
	setFogCameraPosUniform();
}

void FogEffect::setFogrUniform()
{
	if (fogrUniform != -1)
	{
		glUniform1f(fogrUniform, r);
	}
}

void FogEffect::setFogRUniform()
{
	if (fogRUniform != -1)
	{
		glUniform1f(fogRUniform, R);
	}
}

void FogEffect::setFogColorUniform()
{
	if (fogColorUniform != -1)
	{
		glUniform3fv(fogColorUniform, 1, &color.x);
	}
}

void FogEffect::setFogCameraPosUniform()
{
	if (fogCameraPosUniform != -1 && camera)
	{
		glUniform3fv(fogCameraPosUniform, 1, &camera->getPosition().x);
	}
}

void FogEffect::debug()
{
	std::cout << "fog :";
	std::cout << "\t r : " << r << '\n';
	std::cout << "\t R : " << R << '\n';
	std::cout << "\t color : " << color.x << ' ' << color.y << ' ' << color.z << "\n";
}

