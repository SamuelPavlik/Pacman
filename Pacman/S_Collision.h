#pragma once

#include "C_Collision.h"

#include <vector>
#include <memory>
#include <map>

class GameEntity;

class S_Collision
{
public:
    void Add(std::vector<std::shared_ptr<GameEntity>>& entities);
    void ProcessRemovals();
    void Update();
    void Clear();

private:
    std::map<CollisionLayer, std::vector<std::shared_ptr<C_Collision>>> collidables;
};

