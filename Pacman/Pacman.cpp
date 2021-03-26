#include "Pacman.h"
#include "Drawer.h"
#include "Avatar.h"
#include "Ghost.h"
#include "Constants.h"
#include "C_Sprite.h"
#include "C_KeyboardMovement.h"
#include "SDL.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "C_Animation.h"

std::shared_ptr<Pacman> Pacman::Create(Drawer& aDrawer)
{
	std::shared_ptr<Pacman> pacman = std::make_shared<Pacman>(aDrawer);
	pacman->Init();
	return pacman;
}

Pacman::Pacman(Drawer& aDrawer)
: myDrawer(aDrawer)
, myTimeToNextUpdate(0.f)
, myNextMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(3)
, myGhostGhostCounter(0.f)
, myWorld() {}

Pacman::~Pacman(void)
{
}

void Pacman::Init()
{
	//set up world
	myWorld.Init(&myDrawer, myDots, myBigDots);

	//set up edible entities
	entityCollection.Add(myDots);
	entityCollection.Add(myBigDots);

	//set up avatar
	myAvatar = std::make_shared<Avatar>(Vector2f(13 * TILE_SIZE, 22 * TILE_SIZE));
	myAvatar->AddComponent<C_Sprite>(&myDrawer, "open_32.png");
	myAvatar->AddComponent<C_KeyboardMovement>(&input, &myWorld);

	//set up avatar animation
	auto animation = myAvatar->AddComponent<C_Animation>();
	std::shared_ptr<Animation> goingLeftAnim = std::make_shared<Animation>(AnimationState::GoingLeft);
	const float eatingFrameSeconds = 0.2f;
	goingLeftAnim->AddFrame(&myDrawer, "closed_left_32", eatingFrameSeconds);
	goingLeftAnim->AddFrame(&myDrawer, "open_left_32", eatingFrameSeconds);

	std::shared_ptr<Animation> goingRightAnim = std::make_shared<Animation>(AnimationState::GoingRight);
	goingRightAnim->AddFrame(&myDrawer, "closed_right_32", eatingFrameSeconds);
	goingRightAnim->AddFrame(&myDrawer, "open_right_32", eatingFrameSeconds);

	std::shared_ptr<Animation> goingUpAnim = std::make_shared<Animation>(AnimationState::GoingUp);
	goingUpAnim->AddFrame(&myDrawer, "closed_up_32", eatingFrameSeconds);
	goingUpAnim->AddFrame(&myDrawer, "open_up_32", eatingFrameSeconds);

	std::shared_ptr<Animation> goingDownAnim = std::make_shared<Animation>(AnimationState::GoingDown);
	goingDownAnim->AddFrame(&myDrawer, "closed_down_32", eatingFrameSeconds);
	goingDownAnim->AddFrame(&myDrawer, "open_down_32", eatingFrameSeconds);

	animation->AddAnimation(AnimationState::GoingLeft, goingLeftAnim);
	animation->AddAnimation(AnimationState::GoingRight, goingRightAnim);
	animation->AddAnimation(AnimationState::GoingUp, goingUpAnim);
	animation->AddAnimation(AnimationState::GoingDown, goingDownAnim);

	entityCollection.Add(myAvatar);

	//set up ghost
	myGhost = std::make_shared<Ghost>(Vector2f(13 * TILE_SIZE, 13 * TILE_SIZE));
	myGhost->SetWorld(&myWorld);
	auto ghostSprite = myGhost->AddComponent<C_Sprite>(&myDrawer, "ghost_32.png");
	ghostSprite->Load("Ghost_Dead_32.png");
	ghostSprite->Load("Ghost_Vulnerable_32.png");
	entityCollection.Add(myGhost);
}

bool Pacman::Update(float time)
{
	entityCollection.ProcessRemovals();
	entityCollection.ProcessNewEntities();

	if (input.IsKeyDown(Input::Key::Esc))
		return false;

	if (CheckEndGameCondition())
		return false;

	CheckGhostCounter(time);

	entityCollection.Update(time);

	CheckIntersectedDot(myAvatar->GetPosition());
	CheckIntersectedBigDot(myAvatar->GetPosition());
	CheckAvatarGhostCollision();

	if (time > 0)
		myFps = (int) (1 / time);

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

bool Pacman::CheckEndGameCondition()
{
	if (false)
	{
		myDrawer.DrawText("You win!", HUD_FONT, 20, 70);
		return true;
	}
	else if (myLives <= 0)
	{
		myDrawer.DrawText("You lose!", HUD_FONT, 20, 70);
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

			//reset avatar
			myAvatar->SetPosition(Vector2f(AVATAR_START_TILE_X * TILE_SIZE, 
				AVATAR_START_TILE_Y * TILE_SIZE));
			myAvatar->GetComponent<C_KeyboardMovement>()->Start();

			//reset ghost
			myGhost->SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE, 
				GHOST_START_TILE_Y * TILE_SIZE));
			myGhost->Start();
		}
		else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
		{
			myScore += 50;
			//myGhost->MarkForDelete();
			myGhost->Die();
			myGhostGhostCounter = 0.f;
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
	myDrawer.DrawText("Score", HUD_FONT, 20, 50);
	myDrawer.DrawText(scoreStream.str().c_str(), HUD_FONT, 100, 50);

	std::stringstream liveStream;
	liveStream << myLives;
	myDrawer.DrawText("Lives", HUD_FONT, 20, 80);
	myDrawer.DrawText(liveStream.str().c_str(), HUD_FONT, 100, 80);

	myDrawer.DrawText("FPS", HUD_FONT, 880, 50);
	std::stringstream fpsStream;
	fpsStream << myFps;
	myDrawer.DrawText(fpsStream.str().c_str(), HUD_FONT, 930, 50);
}

void Pacman::Draw()
{
	//TODO load world resource
	myWorld.Draw(&myDrawer);

	entityCollection.Draw();

	DrawHUD();

}
