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
	C_Animation(GameEntity& owner);

	void Awake() override;

	void Update(float deltaTime) override;

	void AddAnimation(AnimationState state,
		std::shared_ptr<Animation>& animation);

	void SetAnimationState(AnimationState state);

	const AnimationState GetAnimationState() const;

private:
	std::shared_ptr<C_Sprite> sprite;
	std::unordered_map<AnimationState, std::shared_ptr<Animation>> animations;

	std::pair<AnimationState,
		std::shared_ptr<Animation>> currentAnimation;
};
