#include "Pacman.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"
#include "C_Sprite.h"
#include "C_Animation.h"
#include "C_GhostBehavior.h"
#include "C_KeyboardMovement.h"
#include "SDL.h"
#include "EntityFactory.h"

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

Pacman::Pacman(Drawer& aDrawer) :
	drawer(aDrawer),
	myScore(0),
	myFps(0),
	myLives(3),
	isGameOver(false),
	gameOverText(""),
	myWorld(),
	gameEndCounter(0.f),
	totalPoints() {}

Pacman::~Pacman(void)
{
}

void Pacman::Init()
{
	//add sound resources
	soundManager.AddResource("pacman_chomp.wav");
	soundManager.AddResource("pacman_eatghost.wav");
	soundManager.AddResource("pacman_death.wav");
	soundManager.AddResource("pacman_intermission.wav");
	soundManager.AddResource("coin.wav");

	EntityFactory factory(drawer, input, myWorld);

	//set up world
	myWorld.Init(drawer, factory, dots, bigDots);


	//set up avatar
	std::function<void(CollisionData)> onOverlapFunc = [this](CollisionData cd) {
		OnIntersectedDot(cd);
		OnIntersectedBigDot(cd);
		OnAvatarGhostCollision(cd);
	};
	myAvatar = factory.CreatePacman(
		Vector2f(AVATAR_START_TILE_X * TILE_SIZE, AVATAR_START_TILE_Y * TILE_SIZE), 
		onOverlapFunc, "open_32.png");
	
	//set up ghost
	ghosts.push_back(factory.CreateGhost(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE), Vector2f(), myAvatar, "ghost_32.png"));
	ghosts.push_back(factory.CreateGhost(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE), Vector2f(2.f, 2.f), myAvatar, "ghost_32.png"));
	
	//add all entities to entity collection
	Restart();
}

void Pacman::Restart()
{
	myAvatar->SetPosition(Vector2f(AVATAR_START_TILE_X * TILE_SIZE,
		AVATAR_START_TILE_Y * TILE_SIZE));
	ghosts[0]->SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE));
	ghosts[1]->SetPosition(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE));

	entityCollection.Clear();
	entityCollection.Add(myAvatar);
	entityCollection.Add(ghosts);
	entityCollection.Add(dots);
	entityCollection.Add(bigDots);

	totalPoints = dots.size();
	myLives = 3;
	myScore = 0;
}

bool Pacman::Update(float time)
{
	entityCollection.ProcessRemovals();
	entityCollection.ProcessNewEntities();

	if (input.IsKeyDown(Input::Key::Esc))
		return false;

	entityCollection.Update(time);

	CheckEndGameCondition(time);

	if (time > 0)
		myFps = (int) (1 / time);

	return true;
}

void Pacman::OnIntersectedDot(CollisionData cd)
{
	if (cd.other->tag == DOT_TAG)
	{
		soundManager.Play("coin.wav");
		myScore += SMALL_DOT_POINTS;
		cd.other->SetDelete();
		totalPoints--;
	}
}

void Pacman::OnIntersectedBigDot(CollisionData cd)
{
	if (cd.other->tag == BIG_DOT_TAG)
	{
		soundManager.Play("pacman_chomp.wav");

		//game logic when dot eaten
		myScore += BIG_DOT_POINTS;
		for (auto ghost : ghosts)
		{
			if (auto moveComp = ghost->GetComponent<C_GhostBehavior>())
				moveComp->MarkClaimable();
		}
		
		//delete dot
		cd.other->SetDelete();
	}

}

void Pacman::OnAvatarGhostCollision(CollisionData cd)
{
	if (cd.other->tag == ENEMY_TAG)
	{
		if (auto moveComp = cd.other->GetComponent<C_GhostBehavior>())
		{
			if (moveComp->isDeadFlag)
				return;
			
			if (!moveComp->isClaimableFlag)
			{
				myLives--;

				//reset avatar
				myAvatar->SetPosition(Vector2f(AVATAR_START_TILE_X * TILE_SIZE, 
					AVATAR_START_TILE_Y * TILE_SIZE));
				myAvatar->Start();
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

void Pacman::HasIntersectedCherry()
{
}

void Pacman::CheckEndGameCondition(float time)
{
	if (isGameOver)
	{
		if (gameEndCounter > 0.f)
		{
			gameEndCounter -= time;
		}
		else
		{
			isGameOver = false;
			Restart();
		}
		return;
	}

	bool isWon = totalPoints <= 0;
	bool isLost = myLives <= 0;
	if (isWon || isLost)
	{
		isGameOver = true;
		gameEndCounter = 5.f;
		if (isWon)
		{
			for (auto ghost : ghosts)
				ghost->SetDelete();
			gameOverText = "You win!";
			soundManager.Play("pacman_intermission.wav");
		}
		else if (isLost)
		{
			myAvatar->SetDelete();
			gameOverText = "You lose!";
			soundManager.Play("pacman_death.wav");
		}
	}
}

void Pacman::DrawHUD()
{
	std::stringstream scoreStream;
	scoreStream << myScore;
	drawer.DrawText("Score", HUD_FONT, 20, 50);
	drawer.DrawText(scoreStream.str().c_str(), HUD_FONT, 100, 50);

	std::stringstream liveStream;
	liveStream << myLives;
	drawer.DrawText("Lives", HUD_FONT, 20, 80);
	drawer.DrawText(liveStream.str().c_str(), HUD_FONT, 100, 80);

	drawer.DrawText("FPS", HUD_FONT, 880, 50);
	std::stringstream fpsStream;
	fpsStream << myFps;
	drawer.DrawText(fpsStream.str().c_str(), HUD_FONT, 930, 50);

	if (isGameOver)
	{
		drawer.DrawText(gameOverText, HUD_FONT, 450, 350);
	}
}

void Pacman::Draw()
{
	//TODO load world resource
	myWorld.Draw(&drawer);

	entityCollection.Draw();

	DrawHUD();

}
