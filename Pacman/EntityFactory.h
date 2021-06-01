#pragma once

#include "Vector2f.h"
#include "C_Collision.h"

#include <memory>
#include <functional>

class GameEntity;
class InputManager;
class Drawer;
class World;

class EntityFactory
{
public:
	EntityFactory(Drawer& drawer, InputManager& inputManager, World& world);

	std::shared_ptr<GameEntity> CreatePacman(const Vector2f& position, 
		std::function<void(CollisionData&)> onOverlapFunc,
		const char* spriteName);
	std::shared_ptr<GameEntity> CreateDirectGhost(const Vector2f& position, const Vector2f& spriteOffset,
		std::shared_ptr<GameEntity> avatar, const char* spriteName);
	std::shared_ptr<GameEntity> CreateAmbushGhost(const Vector2f& position, const Vector2f& spriteOffset,
		std::shared_ptr<GameEntity> avatar, const char* spriteName);
	std::shared_ptr<GameEntity> CreateDot(const Vector2f& position, const char* name);
	std::shared_ptr<GameEntity> CreateBigDot(const Vector2f& position, const char* name);

private:
	std::shared_ptr<GameEntity> CreateGhost(std::shared_ptr<GameEntity>& ghost, const char* spriteName);

	Drawer& drawer;
	InputManager& inputManager;
	World& world;
};

