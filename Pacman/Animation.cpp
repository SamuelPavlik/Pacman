#include "Animation.h"
#include "Drawer.h"

Animation::Animation() : 
    frames(0), 
    currentFrameIndex(0),
    currentFrameTime(0.f) {}

void Animation::AddFrame(Drawer* drawer, const char* name, float frameTime) {
    if (!drawer->AddResource(name))
        return;

    FrameData data;
    data.name = name;
    data.displayTimeSeconds = frameTime;
    frames.push_back(data);
}

const FrameData* Animation::GetCurrentFrame() const {
    if (frames.size() > 0) {
        return &frames[currentFrameIndex];
    }

    return nullptr;
}

bool Animation::UpdateFrame(float deltaTime) {
    if (frames.size() > 0) {
        currentFrameTime += deltaTime;

        if (currentFrameTime >=
            frames[currentFrameIndex].displayTimeSeconds) {
            currentFrameTime = 0.f;
            IncrementFrame();
            return true;
        }
    }

    return false;
}

void Animation::Reset() {
    currentFrameIndex = 0;
    currentFrameTime = 0.f;
}

void Animation::IncrementFrame() {
    currentFrameIndex = (currentFrameIndex + 1) % frames.size();
}