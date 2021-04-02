#include "SoundManager.h"

SoundManager::SoundManager()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512);
}

SoundManager::~SoundManager()
{
	for (auto res : resourceMap)
	{
		Mix_FreeChunk(res.second);
	}
	Mix_CloseAudio();
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

void SoundManager::StopAll()
{
	Mix_HaltChannel(-1);
}
