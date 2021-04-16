#include "C_Animation.h"
#include "GameEntity.h"
#include "C_Sprite.h"
#include "Animation.h"

C_Animation::C_Animation(GameEntity& owner) : Component(owner),
currentAnimation(AnimationState::None, nullptr) {}

void C_Animation::Awake() 
{
	sprite = owner.GetComponent<C_Sprite>();
}

void C_Animation::Update(float deltaTime) 
{
	if (currentAnimation.first == AnimationState::None)
	{
		return;
	}

	bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

	if (newFrame) 
	{
		const auto data = *currentAnimation.second->GetCurrentFrame();
		sprite->SetName(data.name);
	}
}

void C_Animation::AddAnimation(AnimationState state, std::shared_ptr<Animation>& animation) 
{
	animations.insert(std::make_pair(state, animation));

	if (currentAnimation.first == AnimationState::None) 
	{
		SetAnimationState(state);
	}
}

void C_Animation::SetAnimationState(AnimationState state) 
{
	if (currentAnimation.first == state) 
	{
		return;
	}

	auto animation = animations.find(state);
	if (animation != animations.end()) 
	{
		currentAnimation.first = animation->first;
		currentAnimation.second = animation->second;
		currentAnimation.second->Reset();
		if (sprite)
			sprite->SetName(animation->second->GetCurrentFrame()->name);
	}
}

const AnimationState& C_Animation::GetAnimationState() const 
{
	return currentAnimation.first;
}