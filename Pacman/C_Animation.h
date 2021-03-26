#ifndef C_ANIMATION_H
#define C_ANIMATION_H

#include "Component.h"
#include "Animation.h"

#include <unordered_map>
#include <memory>

class C_Sprite;

enum class AnimationState {
    None,
    GoingLeft,
    GoingRight,
    GoingUp,
    GoingDown
};

class C_Animation : public Component {
public:
    C_Animation(GameEntity& owner);

    void Awake() override;

    void Update(float deltaTime) override;

    void AddAnimation(AnimationState state,
        std::shared_ptr<Animation> animation);

    void SetAnimationState(AnimationState state);

    const AnimationState& GetAnimationState() const;

private:
    std::shared_ptr<C_Sprite> sprite;
    std::unordered_map<AnimationState, std::shared_ptr<Animation>> animations;

    std::pair<AnimationState,
        std::shared_ptr<Animation>> currentAnimation;
};
#endif /* C_ANIMATION_H */