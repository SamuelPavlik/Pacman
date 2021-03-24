#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"
#include "Constants.h"

const char* HUD_FONT = "freefont-ttf\\sfd\\FreeMono.ttf";

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
, myGhostGhostCounter(0.f)
{
	myAvatar = std::make_shared<Avatar>(Vector2f(13*22,22*22));
	myGhost = std::make_shared<Ghost>(Vector2f(13*22,13*22));
	myWorld = std::make_shared<World>();
}

Pacman::~Pacman(void)
{
}

void Pacman::Init()
{
	myWorld->Init();
}

bool Pacman::Update(float aTime)
{
	if (!UpdateInput())
		return false;

	if (CheckEndGameCondition())
	{
		myDrawer->DrawText("You win!", HUD_FONT, 20, 70);
		return true;
	}
	else if (myLives <= 0)
	{
		myDrawer->DrawText("You lose!", HUD_FONT, 20, 70);	
		return true;
	}

	MoveAvatar();
	myAvatar->Update(aTime);
	myGhost->Update(aTime, myWorld);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		myScore += SMALL_DOT_POINTS;

	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += BIG_DOT_POINTS;
		myGhostGhostCounter = GHOST_COUNTER;
		myGhost->myIsClaimableFlag = true;
	}

	if (myGhostGhostCounter <= 0)
	{
		myGhost->myIsClaimableFlag = false;
	}

	if ((myGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter <= 0.f)
		{
			myLives--;

			myAvatar->SetPosition(Vector2f(13*22,22*22));
			myGhost->SetPosition(Vector2f(13*22,13*22));
		}
		else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
		{
			myScore += 50;
			myGhost->myIsDeadFlag = true;
			myGhost->Die(myWorld);
		}
	}
	
	if (aTime > 0)
		myFps = (int) (1 / aTime);

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
	return false;
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
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	myGhost->Draw(myDrawer);

	DrawHUD();

}
