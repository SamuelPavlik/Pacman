#pragma once

#include <vector>
#include <memory>
#include <set>
#include <functional>

class GameEntity;
class Drawer;

using CompareFuncType = 
std::function<bool(std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>)>;

class S_Sprite
{
public:
    S_Sprite();

    void Add(std::vector<std::shared_ptr<GameEntity>> entities);
    void Add(std::shared_ptr<GameEntity> object);

    void ProcessRemovals();
    void Clear();

    void Draw();

private:
    std::multiset<std::shared_ptr<GameEntity>, CompareFuncType> drawables;
};

