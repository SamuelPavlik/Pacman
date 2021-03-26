#pragma once

#include "Component.h"
#include "Vector2f.h"

#include <memory>

class GameEntity;
class Input;
class World;

class C_KeyboardMovement : public Component
{
public:
    C_KeyboardMovement(GameEntity& owner, Input* input, const World* world);

    void SetMoveSpeed(float moveSpeed);

    void Update(float time) override;
    void Start() override;

private:
    void Move(float time);

    float moveSpeed;
    Input* input;
    const World* world;
    Vector2f nextMovement;

    int myCurrentTileX;
    int myCurrentTileY;
    int myNextTileX;
    int myNextTileY;
};

