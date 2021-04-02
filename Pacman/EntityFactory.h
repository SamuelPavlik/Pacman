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
	EntityFactory(Drawer& drawer, InputManager& input, World& world);
	std::shared_ptr<GameEntity> CreatePacman(Vector2f position, 
		std::function<void(CollisionData)> onOverlapFunc,
		const char* spriteName);
	std::shared_ptr<GameEntity> CreateGhost(Vector2f position, Vector2f spriteOffset, 
		std::shared_ptr<GameEntity> avatar, const char* spriteName);
	std::shared_ptr<GameEntity> CreateDot(Vector2f position, const char* name);
	std::shared_ptr<GameEntity> CreateBigDot(Vector2f position, const char* name);

private:
	Drawer& drawer;
	InputManager& input;
	World& world;
};

