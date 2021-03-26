#include "C_Animation.h"
#include "GameEntity.h"
#include "C_Sprite.h"

C_Animation::C_Animation(GameEntity& owner) : Component(owner),
currentAnimation(AnimationState::None, nullptr) {}

void C_Animation::Awake() {
    sprite = owner.GetComponent<C_Sprite>();
}

void C_Animation::Update(float deltaTime) {
    if (currentAnimation.first != AnimationState::None) {
        bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

        if (newFrame) {
            const FrameData data = *currentAnimation.second->GetCurrentFrame();
            sprite->Load(data.name);
        }
    }
}

void C_Animation::AddAnimation(AnimationState state, std::shared_ptr<Animation> animation) {
    animations.insert(std::make_pair(state, animation));

    if (currentAnimation.first == AnimationState::None) {
        SetAnimationState(state);
    }
}

void C_Animation::SetAnimationState(AnimationState state) {
    if (currentAnimation.first == state) {
        return;
    }

    auto animation = animations.find(state);
    if (animation != animations.end()) {
        currentAnimation.first = animation->first;
        currentAnimation.second = animation->second;
        currentAnimation.second->Reset();
    }
}

const AnimationState& C_Animation::GetAnimationState() const {
    return currentAnimation.first;
}