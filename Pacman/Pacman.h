#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "World.h"
#include "SoundManager.h"

#include <memory>

struct SDL_Surface;
class Drawer;
class Avatar;
class Ghost;

using GameCollection = std::vector<std::shared_ptr<GameEntity>>;
using GameIt = GameCollection::iterator;

class Pacman
{
public:
	static std::shared_ptr<Pacman> Create(Drawer& aDrawer);
	Pacman(Drawer& aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	void Draw();

private:
	void Init();
	void Restart();

	void OnIntersectedDot(CollisionData cd);
	void OnIntersectedBigDot(CollisionData cd);
	void OnAvatarGhostCollision(CollisionData cd);
	void HasIntersectedCherry();
	void CheckEndGameCondition(float time);

	void DrawHUD();

	int myLives;
	int myScore;
	int myFps;
	int totalPoints;
	float gameEndCounter;
	bool isGameOver;
	const char* gameOverText;

	//game entities
	std::shared_ptr<GameEntity> myAvatar;
	std::vector<std::shared_ptr<GameEntity>> ghosts;
	std::vector<std::shared_ptr<GameEntity>> dots;
	std::vector<std::shared_ptr<GameEntity>> bigDots;

	World myWorld;

	//systems
	EntityManager entityCollection;
	InputManager input;
	SoundManager soundManager;
	Drawer& drawer;
};

#endif // PACMAN_H