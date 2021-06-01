#include "Animation.h"
#include "Drawer.h"

Animation::Animation() noexcept : 
	frames(0), 
	currentFrameIndex(0),
	currentFrameTime(0.f) {}

void Animation::AddFrame(Drawer& drawer, const char* name, float frameTime) 
{
	if (!drawer.AddResource(name))
	{
		return;
	}

	FrameData data;
	data.name = name;
	data.displayTimeSeconds = frameTime;
	frames.push_back(data);
}

const FrameData* Animation::GetCurrentFrame() const noexcept
{
	if (frames.size() > 0) 
	{
		return &frames[currentFrameIndex];
	}

	return nullptr;
}

bool Animation::UpdateFrame(float deltaTime) noexcept
{
	if (frames.empty()) 
	{
		return false;
	}

	//in case it is a one frame animation
	if (frames.size() == 1)
	{
		if (currentFrameTime == 0)
		{
			return true;
		}
		currentFrameTime += deltaTime;
		return false;
	}
	currentFrameTime += deltaTime;

	if (currentFrameTime >=
		frames[currentFrameIndex].displayTimeSeconds) 
	{
		currentFrameTime = 0.f;
		IncrementFrame();
		return true;
	}

	return false;
}

void Animation::Reset() noexcept
{
	currentFrameIndex = 0;
	currentFrameTime = 0.f;
}

void Animation::IncrementFrame() noexcept 
{
	currentFrameIndex = (currentFrameIndex + 1) % frames.size();
}