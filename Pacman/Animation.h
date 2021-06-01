#pragma once

#include <vector>
#include <string>

class Drawer;

struct FrameData 
{
	const char* name;
	float displayTimeSeconds;
};

class Animation 
{
public:
	Animation() noexcept;

	void AddFrame(Drawer& drawer, const char* name, float frameTime);
	const FrameData* GetCurrentFrame() const noexcept;
	bool UpdateFrame(float deltaTime) noexcept;

	void Reset() noexcept;

private:
	void IncrementFrame() noexcept;

	std::vector<FrameData> frames;
	int currentFrameIndex;
	float currentFrameTime;
};