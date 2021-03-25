#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "EntityCollection.h"

#include <memory>

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class Ghost;

class Pacman
{
public:
	static std::shared_ptr<Pacman> Create(std::shared_ptr<Drawer> aDrawer);
	Pacman(std::shared_ptr<Drawer> aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	void Draw();

private:
	void Init();
	bool UpdateInput();

	bool CheckIntersectedDot(const Vector2f& aPosition);
	bool CheckIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);
	void MoveAvatar();
	bool CheckEndGameCondition();
	void CheckAvatarGhostCollision();
	void CheckGhostCounter(float time);

	void DrawHUD();

	std::shared_ptr<Drawer> myDrawer;

	float myTimeToNextUpdate;
	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;

	std::shared_ptr<Avatar> myAvatar;
	std::shared_ptr<Ghost> myGhost;
	std::vector<std::shared_ptr<GameEntity>> myDots;
	std::vector<std::shared_ptr<GameEntity>> myBigDots;
	std::vector<std::shared_ptr<GameEntity>> myCherries;
	std::shared_ptr<World> myWorld;
	EntityCollection entityCollection;

};

#endif // PACMAN_H