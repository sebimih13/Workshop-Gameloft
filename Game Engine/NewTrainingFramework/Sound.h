#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"

#include "fmod.hpp"

struct SoundResource
{
	int id;					// TODO : do i need this?
	std::string filePath;
};

class Sound
{
public:
	/** Constructor */
	Sound(SoundResource* soundResource);

	/** Destructor */
	~Sound();

	/** Load */
	void Load(FMOD::System* system);

	/** Play stored sound */
	void Play();

private:
	SoundResource* resource;

	FMOD::System* fmodSystem;
	FMOD::Sound* sound;
};

