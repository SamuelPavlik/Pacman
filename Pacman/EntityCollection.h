#pragma once

#include "S_Sprite.h"

#include <memory>
#include <vector>

class Drawer;
class GameEntity;
struct SDL_Texture;

class EntityCollection
{
public:
    void Add(std::shared_ptr<GameEntity> entity);
    void Add(std::vector<std::shared_ptr<GameEntity>> entities);

    void Update(float deltaTime);
    void Draw(std::shared_ptr<Drawer> drawer);

    void ProcessNewEntities();
    void ProcessRemovals();

private:
    S_Sprite spriteSys;
    std::vector<std::shared_ptr<GameEntity>> entities;
    std::vector<std::shared_ptr<GameEntity>> newEntities;

};