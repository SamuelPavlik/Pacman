#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

#include <unordered_map>
#include <tuple>

using SoundResource = std::tuple<Uint8*, Uint32>;

class SoundManager
{
public:
	SoundManager();
	void AddResource(const char* name);
	void Play(const char* name);

private:
	SDL_AudioSpec wavSpec;
	SDL_AudioDeviceID deviceId;

	std::unordered_map<const char*, Mix_Chunk*> resourceMap;
};

