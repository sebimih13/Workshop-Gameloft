#include "stdafx.h"
#include "Sound.h"

#include "ResourceManager.h"

Sound::Sound(SoundResource* soundResource)
	: resource(soundResource)
{
	// TODO : le mai scriu
	sound = nullptr;
	fmodSystem = nullptr;
}

Sound::~Sound()
{
	sound->release();
	delete resource;
}

void Sound::Load(FMOD::System* system)
{
	fmodSystem = system;
	fmodSystem->createSound(resource->filePath.c_str(), FMOD_LOOP_OFF, 0, &sound);
}

void Sound::Play()
{
	if (fmodSystem && sound)
	{
		fmodSystem->playSound(sound, 0, false, 0);
	}
}

