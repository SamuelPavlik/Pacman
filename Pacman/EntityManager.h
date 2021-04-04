#pragma once

#include "S_Sprite.h"
#include "S_Collision.h"

#include <memory>
#include <vector>

class Drawer;
class GameEntity;
struct SDL_Texture;

class EntityManager
{
public:
	void Add(std::shared_ptr<GameEntity> entity);
	void Add(std::vector<std::shared_ptr<GameEntity>> entities);

	void Update(float deltaTime);
	void Draw();

	void ProcessNewEntities();
	void ProcessRemovals();
	void Clear();

private:
	S_Sprite spriteSys;
	S_Collision collisionSys;
	std::vector<std::shared_ptr<GameEntity>> entities;
	std::vector<std::shared_ptr<GameEntity>> newEntities;

};