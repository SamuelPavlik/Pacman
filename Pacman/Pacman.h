#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "EntityCollection.h"
#include "Input.h"
#include "World.h"

#include <memory>

struct SDL_Surface;
class Drawer;
class Avatar;
class Ghost;

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

	bool CheckIntersectedDot(const Vector2f& aPosition);
	bool CheckIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);
	bool CheckEndGameCondition();
	void CheckAvatarGhostCollision();

	void DrawHUD();

	int myLives;
	int myScore;
	int myFps;

	float gameEndCounter;

	//game entities
	std::shared_ptr<GameEntity> myAvatar;
	std::shared_ptr<GameEntity> myGhost;
	std::vector<std::shared_ptr<GameEntity>> myDots;
	std::vector<std::shared_ptr<GameEntity>> myBigDots;
	std::vector<std::shared_ptr<GameEntity>> myCherries;

	World myWorld;

	//systems
	EntityCollection entityCollection;
	Input input;
	Drawer& myDrawer;
};

#endif // PACMAN_H