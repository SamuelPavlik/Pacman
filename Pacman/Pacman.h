#pragma once

#include "Vector2f.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "World.h"
#include "SoundManager.h"
#include "Menu.h"

#include <memory>
#include <string>

struct SDL_Surface;
class Drawer;
class Avatar;
class Ghost;

using GameCollection = std::vector<std::shared_ptr<GameEntity>>;
using GameIt = GameCollection::iterator;

class Pacman
{
public:
	static std::shared_ptr<Pacman> Create(Drawer& drawer);
	Pacman(Drawer& drawer);

	bool Update(float aTime);
	void Draw();

private:
	void Init();
	void Restart();

	void OnIntersectedDot(CollisionData cd);
	void OnIntersectedBigDot(CollisionData cd);
	void OnAvatarGhostCollision(CollisionData cd);
	void CheckEndGameCondition(float time);

	void DrawHUD();

	int lives;
	int score;
	int bestScore;
	int fps;
	int totalPoints;
	float gameEndCounter;
	bool isGameOver;
	MenuState menuState;
	std::string gameOverText;

	//game entities
	std::shared_ptr<GameEntity> avatar;
	std::vector<std::shared_ptr<GameEntity>> ghosts;
	std::vector<std::shared_ptr<GameEntity>> dots;
	std::vector<std::shared_ptr<GameEntity>> bigDots;

	World world;

	//systems
	EntityManager entityManager;
	InputManager inputManager;
	SoundManager soundManager;
	Menu menu;
	Drawer& drawer;
};