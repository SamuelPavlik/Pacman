#include "Pacman.h"
#include "Drawer.h"
#include "Avatar.h"
#include "World.h"
#include "Ghost.h"
#include "Constants.h"
#include "C_Sprite.h"
#include "SDL.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

std::shared_ptr<Pacman> Pacman::Create(std::shared_ptr<Drawer> aDrawer)
{
	std::shared_ptr<Pacman> pacman = std::make_shared<Pacman>(aDrawer);
	pacman->Init();
	return pacman;
}

Pacman::Pacman(std::shared_ptr<Drawer> aDrawer)
: myDrawer(aDrawer)
, myTimeToNextUpdate(0.f)
, myNextMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(3)
, myGhostGhostCounter(0.f){}

Pacman::~Pacman(void)
{
}

void Pacman::Init()
{
	//set up world
	myWorld = std::make_shared<World>();
	myWorld->Init(myDrawer, myDots, myBigDots);

	//set up edible entities
	entityCollection.Add(myDots);
	entityCollection.Add(myBigDots);

	//set up avatar
	myAvatar = std::make_shared<Avatar>(Vector2f(13 * TILE_SIZE, 22 * TILE_SIZE));
	myAvatar->SetSprite(myDrawer, "open_32.png");
	entityCollection.Add(myAvatar);

	//set up ghost
	myGhost = std::make_shared<Ghost>(Vector2f(13 * TILE_SIZE, 13 * TILE_SIZE));
	myGhost->SetWorld(myWorld);
	myGhost->SetSprite(myDrawer, "ghost_32.png");
	myGhost->GetSprite()->Load(myDrawer, "Ghost_Dead_32.png");
	myGhost->GetSprite()->Load(myDrawer, "Ghost_Vulnerable_32.png");
	entityCollection.Add(myGhost);
}

bool Pacman::Update(float time)
{
	entityCollection.ProcessRemovals();
	entityCollection.ProcessNewEntities();

	if (!UpdateInput())
		return false;

	if (CheckEndGameCondition())
		return false;

	CheckGhostCounter(time);

	MoveAvatar();
	entityCollection.Update(time);

	CheckIntersectedDot(myAvatar->GetPosition());
	CheckIntersectedBigDot(myAvatar->GetPosition());
	CheckAvatarGhostCollision();

	if (time > 0)
		myFps = (int) (1 / time);

	return true;
}

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
		myNextMovement = Vector2f(0.f, -1.f);
	else if (keystate[SDL_SCANCODE_DOWN])
		myNextMovement = Vector2f(0.f, 1.f);
	else if (keystate[SDL_SCANCODE_RIGHT])
		myNextMovement = Vector2f(1.f, 0.f);
	else if (keystate[SDL_SCANCODE_LEFT])
		myNextMovement = Vector2f(-1.f, 0.f);

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

bool Pacman::CheckIntersectedDot(const Vector2f& aPosition)
{
	auto dotIt = std::find_if(myDots.begin(), myDots.end(), [&aPosition](auto dot) {
		return (dot->GetPosition() - aPosition).Length() < 5.f;
		});
	if (dotIt != myDots.end())
	{
		//game logic when dot eaten
		myScore += SMALL_DOT_POINTS;

		//delete dot
		(*dotIt)->MarkForDelete();
		myDots.erase(dotIt);
		return true;
	}
	return false;
}

bool Pacman::CheckIntersectedBigDot(const Vector2f& aPosition)
{
	auto bigDotIt = std::find_if(myBigDots.begin(), myBigDots.end(), [&aPosition](auto dot) {
		return (dot->GetPosition() - aPosition).Length() < 5.f;
		});
	if (bigDotIt != myBigDots.end())
	{
		//game logic when dot eaten
		myScore += BIG_DOT_POINTS;
		myGhostGhostCounter = GHOST_COUNTER;
		myGhost->myIsClaimableFlag = true;
		
		//delete dot
		(*bigDotIt)->MarkForDelete();
		myBigDots.erase(bigDotIt);
		return true;
	}

	return false;
}

bool Pacman::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void Pacman::MoveAvatar()
{
	int nextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
	int nextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;

	if (myAvatar->IsAtDestination())
	{
		if (myWorld->TileIsValid(nextTileX, nextTileY))
		{
			myAvatar->SetNextTile(nextTileX, nextTileY);
		}
	}
}

bool Pacman::CheckEndGameCondition()
{
	if (false)
	{
		myDrawer->DrawText("You win!", HUD_FONT, 20, 70);
		return true;
	}
	else if (myLives <= 0)
	{
		myDrawer->DrawText("You lose!", HUD_FONT, 20, 70);
		return true;
	}

	return false;
}

void Pacman::CheckAvatarGhostCollision()
{
	if ((myGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter <= 0.f)
		{
			myLives--;

			myAvatar->SetPosition(Vector2f(13 * 22, 22 * 22));
			myGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
		}
		else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
		{
			myScore += 50;
			myGhost->myIsDeadFlag = true;
			myGhost->Die();
		}
	}
}

void Pacman::CheckGhostCounter(float time)
{
	if (myGhostGhostCounter <= 0)
	{
		myGhost->myIsClaimableFlag = false;
	}
	myGhostGhostCounter -= time;
}

void Pacman::DrawHUD()
{
	std::stringstream scoreStream;
	scoreStream << myScore;
	myDrawer->DrawText("Score", HUD_FONT, 20, 50);
	myDrawer->DrawText(scoreStream.str().c_str(), HUD_FONT, 100, 50);

	std::stringstream liveStream;
	liveStream << myLives;
	myDrawer->DrawText("Lives", HUD_FONT, 20, 80);
	myDrawer->DrawText(liveStream.str().c_str(), HUD_FONT, 100, 80);

	myDrawer->DrawText("FPS", HUD_FONT, 880, 50);
	std::stringstream fpsStream;
	fpsStream << myFps;
	myDrawer->DrawText(fpsStream.str().c_str(), HUD_FONT, 930, 50);
}

void Pacman::Draw()
{
	//TODO load world resource
	myWorld->Draw(myDrawer);

	entityCollection.Draw(myDrawer);

	DrawHUD();

}
