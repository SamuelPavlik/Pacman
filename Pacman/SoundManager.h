#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

#include <unordered_map>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void AddResource(const char* name);
	void Play(const char* name);

private:
	std::unordered_map<const char*, Mix_Chunk*> resourceMap;
};

