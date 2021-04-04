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
	Animation();

	void AddFrame(Drawer& drawer, const char* name, float frameTime);
	const FrameData* GetCurrentFrame() const;
	bool UpdateFrame(float deltaTime);

	void Reset();

private:
	void IncrementFrame();

	std::vector<FrameData> frames;
	int currentFrameIndex;
	float currentFrameTime;
};