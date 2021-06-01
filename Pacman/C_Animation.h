#pragma once

#include "Component.h"

#include <unordered_map>
#include <memory>

class C_Sprite;
class Animation;

enum class AnimationState 
{
	None,
	GoingLeft,
	GoingRight,
	GoingUp,
	GoingDown,
	Dead,
	Vulnerable
};

class C_Animation : public Component 
{
public:
	C_Animation(GameEntity& owner) noexcept;

	void Awake() override;
	void Update(float deltaTime) override;

	template<typename T>
	void AddAnimation(AnimationState state,	T&& animation)
	{
		animations.insert(std::make_pair(state, std::forward<T>(animation)));

		if (currentAnimation.first == AnimationState::None)	{
			SetAnimationState(state);
		}
	}
	void SetAnimationState(AnimationState state);
	const AnimationState GetAnimationState() const noexcept;

private:
	std::shared_ptr<C_Sprite> sprite;
	std::unordered_map<AnimationState, std::shared_ptr<Animation>> animations;

	std::pair<AnimationState,
		std::shared_ptr<Animation>> currentAnimation;
};
