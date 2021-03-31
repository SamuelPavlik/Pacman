#include "Pacman.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"
#include "C_Sprite.h"
#include "C_Animation.h"
#include "C_GhostBehavior.h"
#include "C_KeyboardMovement.h"
#include "SDL.h"
#include "Collisions.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

std::shared_ptr<Pacman> Pacman::Create(Drawer& aDrawer)
{
	std::shared_ptr<Pacman> pacman = std::make_shared<Pacman>(aDrawer);
	pacman->Init();
	return pacman;
}

Pacman::Pacman(Drawer& aDrawer)
: myDrawer(aDrawer)
, myScore(0)
, myFps(0)
, myLives(3)
, isGameOver(false)
, gameOverText("")
, myWorld()
, gameEndCounter (0.f) {}

Pacman::~Pacman(void)
{
}

void Pacman::Init()
{
	soundManager.AddResource("pacman_chomp.wav");
	soundManager.AddResource("pacman_eatghost.wav");
	soundManager.AddResource("pacman_death.wav");
	soundManager.AddResource("pacman_intermission.wav");
	soundManager.AddResource("coin.wav");

	//set up world
	myWorld.Init(&myDrawer, myDots, myBigDots);

	//set up edible entities
	entityCollection.Add(myDots);
	entityCollection.Add(myBigDots);

	//set up avatar
	myAvatar = std::make_shared<GameEntity>(Vector2f(AVATAR_START_TILE_X * TILE_SIZE, 
		AVATAR_START_TILE_Y * TILE_SIZE));
	myAvatar->AddComponent<C_Sprite>(&myDrawer, "open_32.png");
	myAvatar->AddComponent<C_KeyboardMovement>(&input, &myWorld);

	//set up avatar animation
	auto avatarAnim = myAvatar->AddComponent<C_Animation>();
	auto goingLeftAnim = std::make_shared<Animation>();
	const float eatingFrameSeconds = 0.2f;
	goingLeftAnim->AddFrame(&myDrawer, "closed_left_32.png", eatingFrameSeconds);
	goingLeftAnim->AddFrame(&myDrawer, "open_left_32.png", eatingFrameSeconds);

	auto goingRightAnim = std::make_shared<Animation>();
	goingRightAnim->AddFrame(&myDrawer, "closed_right_32.png", eatingFrameSeconds);
	goingRightAnim->AddFrame(&myDrawer, "open_right_32.png", eatingFrameSeconds);

	auto goingUpAnim = std::make_shared<Animation>();
	goingUpAnim->AddFrame(&myDrawer, "closed_up_32.png", eatingFrameSeconds);
	goingUpAnim->AddFrame(&myDrawer, "open_up_32.png", eatingFrameSeconds);

	auto goingDownAnim = std::make_shared<Animation>();
	goingDownAnim->AddFrame(&myDrawer, "closed_down_32.png", eatingFrameSeconds);
	goingDownAnim->AddFrame(&myDrawer, "open_down_32.png", eatingFrameSeconds);

	avatarAnim->AddAnimation(AnimationState::GoingLeft, goingLeftAnim);
	avatarAnim->AddAnimation(AnimationState::GoingRight, goingRightAnim);
	avatarAnim->AddAnimation(AnimationState::GoingUp, goingUpAnim);
	avatarAnim->AddAnimation(AnimationState::GoingDown, goingDownAnim);

	entityCollection.Add(myAvatar);

	//set up ghost
	myGhost = std::make_shared<GameEntity>(Vector2f(GHOST_START_TILE_X * TILE_SIZE, 
		GHOST_START_TILE_Y * TILE_SIZE));
	myGhost->AddComponent<C_Sprite>(&myDrawer, "ghost_32.png");
	myGhost->AddComponent<C_GhostBehavior>(&myWorld, myAvatar);
	auto ghostAnim = myGhost->AddComponent<C_Animation>();
	
	auto normalAnim = std::make_shared<Animation>();
	normalAnim->AddFrame(&myDrawer, "ghost_32.png", 0.f);

	auto vulnerableAnim = std::make_shared<Animation>();
	vulnerableAnim->AddFrame(&myDrawer, "Ghost_Vulnerable_32.png", 0.f);

	auto deadAnim = std::make_shared<Animation>();
	deadAnim->AddFrame(&myDrawer, "Ghost_Dead_32.png", 0.f);

	ghostAnim->AddAnimation(AnimationState::GoingUp, normalAnim);
	ghostAnim->AddAnimation(AnimationState::Vulnerable, vulnerableAnim);
	ghostAnim->AddAnimation(AnimationState::Dead, deadAnim);
	
	entityCollection.Add(myGhost);
}

bool Pacman::Update(float time)
{
	entityCollection.ProcessRemovals();
	entityCollection.ProcessNewEntities();

	if (input.IsKeyDown(Input::Key::Esc))
		return false;

	entityCollection.Update(time);

	CheckIntersectedDot();
	CheckIntersectedBigDot();
	CheckAvatarGhostCollision();
	CheckEndGameCondition();

	if (time > 0)
		myFps = (int) (1 / time);

	return true;
}

void Pacman::CheckIntersectedDot()
{
	GameIt dotIt;
	if (Collisions::IsColliding(myDots, myAvatar, dotIt))
	{
		soundManager.Play("coin.wav");

		//game logic when dot eaten
		myScore += SMALL_DOT_POINTS;

		//delete dot
		(*dotIt)->MarkForDelete();
		myDots.erase(dotIt);
	}
}

void Pacman::CheckIntersectedBigDot()
{
	GameIt bigDotIt;
	if (Collisions::IsColliding(myBigDots, myAvatar, bigDotIt))
	{
		soundManager.Play("pacman_chomp.wav");

		//game logic when dot eaten
		myScore += BIG_DOT_POINTS;
		if (auto moveComp = myGhost->GetComponent<C_GhostBehavior>())
			moveComp->MarkClaimable();
		
		//delete dot
		(*bigDotIt)->MarkForDelete();
		myBigDots.erase(bigDotIt);
	}

}

void Pacman::HasIntersectedCherry()
{
}

void Pacman::CheckEndGameCondition()
{
	static bool isPlaying = false;
	if (myDots.empty())
	{
		myGhost->MarkForDelete();
		gameOverText = "You win!";
		isGameOver = true;
		if (!isPlaying)
		{
			soundManager.Play("pacman_intermission.wav");
			isPlaying = true;
		}
	}
	else if (myLives <= 0)
	{
		gameOverText = "You lose!";
		myAvatar->MarkForDelete();
		isGameOver = true;
	}
}

void Pacman::CheckAvatarGhostCollision()
{
	if (Collisions::IsColliding(myAvatar, myGhost))
	{
		if (auto moveComp = myGhost->GetComponent<C_GhostBehavior>())
		{
			if (moveComp->isDeadFlag)
				return;
			
			if (!moveComp->isClaimableFlag)
			{
				myLives--;
				if (myLives <= 0)
					soundManager.Play("pacman_death.wav");

				//reset avatar
				myAvatar->SetPosition(Vector2f(AVATAR_START_TILE_X * TILE_SIZE, 
					AVATAR_START_TILE_Y * TILE_SIZE));
				myAvatar->Start();

				//reset ghost
				myGhost->SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE, 
					GHOST_START_TILE_Y * TILE_SIZE));
				myGhost->Start();
			}
			else
			{
				soundManager.Play("pacman_eatghost.wav");
				moveComp->Die();
				myScore += 50;
			}
		}
	}
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

	if (isGameOver)
	{
		myDrawer.DrawText(gameOverText, HUD_FONT, 450, 350);
	}
}

void Pacman::Draw()
{
	//TODO load world resource
	myWorld.Draw(&myDrawer);

	entityCollection.Draw();

	DrawHUD();

}
