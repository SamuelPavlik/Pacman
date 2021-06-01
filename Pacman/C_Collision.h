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
    GameEntity* other = nullptr;
};

class C_Collision : public Component
{
public:
    C_Collision(GameEntity& owner, CollisionLayer layer);

    CollisionData C_Collision::IsColliding(const std::shared_ptr<const C_Collision>& other) const noexcept;
    void Resolve(CollisionData& cd);
    void BindOnOverlapFunc(std::function<void(CollisionData& cd)>& onOverlapFunc) noexcept;
    void BindOnOverlapFunc(std::function<void(CollisionData& cd)>&& onOverlapFunc) noexcept;

    CollisionLayer layer;

private:
    std::function<void(CollisionData& cd)> onOverlapFunc;
};

