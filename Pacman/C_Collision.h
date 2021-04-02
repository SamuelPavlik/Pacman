#pragma once

#include "Component.h"

#include <memory>
#include <functional>

class GameEntity;

enum class CollisionLayer
{
    Player = 1,
    NonPlayer = 2
};

struct CollisionData
{
    bool colliding = false;
    GameEntity* other = NULL;
};

class C_Collision : public Component
{
public:
    C_Collision(GameEntity& owner, CollisionLayer layer);

    CollisionData C_Collision::IsColliding(std::shared_ptr<C_Collision> other);
    void Resolve(CollisionData cd);

    void BindOnOverlapFunc(std::function<void(CollisionData cd)> onOverlapFunc);

    CollisionLayer layer;

private:
    std::function<void(CollisionData cd)> onOverlapFunc;
};

