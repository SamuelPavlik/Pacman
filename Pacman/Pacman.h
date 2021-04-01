#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "EntityCollection.h"
#include "Input.h"
#include "World.h"
#include "SoundManager.h"
#include "C_Collision.h"

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

	void OnIntersectedDot(CollisionData cd);
	void OnIntersectedBigDot(CollisionData cd);
	void OnAvatarGhostCollision(CollisionData cd);
	void HasIntersectedCherry();
	void CheckEndGameCondition();

	void DrawHUD();

	int myLives;
	int myScore;
	int myFps;
	int totalPoints;
	bool isGameOver;
	const char* gameOverText;

	float gameEndCounter;

	//game entities
	std::shared_ptr<GameEntity> myAvatar;
	std::shared_ptr<GameEntity> myGhost;

	World myWorld;

	//systems
	EntityCollection entityCollection;
	Input input;
	Drawer& myDrawer;
	SoundManager soundManager;
};

#endif // PACMAN_H