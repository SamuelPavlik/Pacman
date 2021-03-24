#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"

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
	void MoveAvatar();
	bool CheckEndGameCondition();
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
	std::shared_ptr<World> myWorld;

};

#endif // PACMAN_H