#include "SoundManager.h"

SoundManager::SoundManager()
{
	//deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512);
}

void SoundManager::AddResource(const char* name)
{
	if (resourceMap.find(name) == resourceMap.end())
	{
		resourceMap[name] = Mix_LoadWAV(name);
	}
}

void SoundManager::Play(const char* name)
{
	if (resourceMap.find(name) != resourceMap.end())
	{
		Mix_PlayChannel(-1, resourceMap[name], 0);
	}
}