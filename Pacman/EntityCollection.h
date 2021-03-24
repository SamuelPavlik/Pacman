#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "S_Sprite.h"

class Drawer;
class GameEntity;
struct SDL_Texture;

class EntityCollection
{
public:
    EntityCollection(std::shared_ptr<S_Sprite> spriteSys);

    void Add(std::shared_ptr<GameEntity> entity);
    void Add(std::vector<std::shared_ptr<GameEntity>> entities);

    void Update(float deltaTime);
    void LateUpdate(float deltaTime);
    void Draw(std::shared_ptr<Drawer> aDrawer);

    void ProcessNewEntities();
    void ProcessNewResources(std::vector<std::shared_ptr<GameEntity>> entities);
    void ProcessRemovals();

private:
    std::shared_ptr<S_Sprite> spriteSys;
    std::vector<std::shared_ptr<GameEntity>> entities;
    std::vector<std::shared_ptr<GameEntity>> newEntities;

};