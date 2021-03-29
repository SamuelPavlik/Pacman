#pragma once

#include "Component.h"
#include "Vector2f.h"
#include "Constants.h"

#include <memory>

class GameEntity;
class Input;
class World;
class C_Animation;

class C_KeyboardMovement : public Component
{
public:
    C_KeyboardMovement(GameEntity& owner, Input* input, const World* world, float moveSpeed = AVATAR_SPEED);

    void SetMoveSpeed(float moveSpeed);

    void Awake() override;
    void Start() override;
    void Update(float time) override;

private:
    void Move(float time, Vector2f possibleMove);

    float moveSpeed;
    Input* input;
    const World* world;
    Vector2f nextMovement;
    std::shared_ptr<C_Animation> animation;

    int myCurrentTileX;
    int myCurrentTileY;
    int myNextTileX;
    int myNextTileY;
};

