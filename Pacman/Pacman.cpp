#include "Pacman.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"
#include "C_GhostBehavior.h"
#include "EntityFactory.h"
#include "SDL.h"

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
	menuState(MenuState::None),
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
	soundManager.AddResource(DOT_SOUND);
	soundManager.AddResource(BIG_DOT_SOUND);
	soundManager.AddResource(EAT_GHOST_SOUND);
	soundManager.AddResource(PAC_DEATH_SOUND);
	soundManager.AddResource(PAC_WON_SOUND);
	soundManager.AddResource(MENU_SOUND);

	//set up factory for creating new game entities
	EntityFactory factory(drawer, inputManager, myWorld);

	//set up world
	myWorld.Init(drawer, factory, dots, bigDots);

	//set up avatar
	std::function<void(CollisionData)> onOverlapFunc = [this](CollisionData cd) {
		OnIntersectedDot(cd);
		OnIntersectedBigDot(cd);
		OnAvatarGhostCollision(cd);
	};
	myAvatar = factory.CreatePacman(
		Vector2f(PACMAN_START_TILE_X * TILE_SIZE, PACMAN_START_TILE_Y * TILE_SIZE), 
		onOverlapFunc, "open_32.png");
	
	//set up ghost
	ghosts.push_back(factory.CreateGhost(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE), Vector2f(), myAvatar, "ghost_32.png"));
	ghosts.push_back(factory.CreateGhost(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE), Vector2f(4.f, 4.f), myAvatar, "ghost_32.png"));
	
	//add all entities to entity collection
	Restart();
}

void Pacman::Restart()
{
	myAvatar->SetPosition(Vector2f(PACMAN_START_TILE_X * TILE_SIZE,
		PACMAN_START_TILE_Y * TILE_SIZE));
	ghosts[0]->SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE));
	ghosts[1]->SetPosition(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE));

	entityManager.Clear();
	entityManager.Add(dots);
	entityManager.Add(bigDots);
	entityManager.Add(ghosts);
	entityManager.Add(myAvatar);

	totalPoints = dots.size();
	myLives = 3;
	myScore = 0;

	soundManager.Play(MENU_SOUND);
	menuState = MenuState::None;
}

bool Pacman::Update(float time)
{
	if (menuState == MenuState::None)
	{
		menuState = menu.Update(inputManager);
		if (menuState == MenuState::None)
			return true;
		else if (menuState == MenuState::Quit)
			return false;
		else
			soundManager.StopAll();
	}

	entityManager.ProcessRemovals();
	entityManager.ProcessNewEntities();

	if (inputManager.IsKeyDown(InputManager::Key::Esc))
		return false;

	entityManager.Update(time);

	CheckEndGameCondition(time);

	if (time > 0)
		myFps = (int) (1 / time);

	return true;
}

void Pacman::OnIntersectedDot(CollisionData cd)
{
	if (cd.other->tag == DOT_TAG)
	{
		soundManager.Play(DOT_SOUND);
		myScore += SMALL_DOT_POINTS;
		cd.other->SetDelete();
		totalPoints--;
	}
}

void Pacman::OnIntersectedBigDot(CollisionData cd)
{
	if (cd.other->tag == BIG_DOT_TAG)
	{
		soundManager.Play(BIG_DOT_SOUND);

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
				myAvatar->SetPosition(Vector2f(PACMAN_START_TILE_X * TILE_SIZE, 
					PACMAN_START_TILE_Y * TILE_SIZE));
				myAvatar->Start();
			}
			else
			{
				soundManager.Play(EAT_GHOST_SOUND);
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
			soundManager.Play(PAC_WON_SOUND);
		}
		else if (isLost)
		{
			myAvatar->SetDelete();
			gameOverText = "You lose!";
			soundManager.Play(PAC_DEATH_SOUND);
		}
	}
}

void Pacman::DrawHUD()
{
	std::stringstream scoreStream;
	scoreStream << myScore;
	drawer.DrawText("Score", HUD_FONT, SCORE_X, SCORE_Y);
	drawer.DrawText(scoreStream.str().c_str(), HUD_FONT, SCORE_NUM_X, SCORE_Y);

	std::stringstream liveStream;
	liveStream << myLives;
	drawer.DrawText("Lives", HUD_FONT, LIVES_X, LIVES_Y);
	drawer.DrawText(liveStream.str().c_str(), HUD_FONT, LIVES_NUM_X, LIVES_Y);

	std::stringstream fpsStream;
	fpsStream << myFps;
	drawer.DrawText("FPS", HUD_FONT, FPS_X, FPS_Y);
	drawer.DrawText(fpsStream.str().c_str(), HUD_FONT, FPS_NUM_X, FPS_Y);

	if (isGameOver)
	{
		drawer.DrawText(gameOverText.c_str(), 
			HUD_FONT, WINDOW_WIDTH / 2 - (gameOverText.size() / 2) * 15, START_Y);
	}
}

void Pacman::Draw()
{
	myWorld.Draw(&drawer);
	if (menuState == MenuState::None)
	{
		menu.Draw(drawer);
		return;
	}

	entityManager.Draw();
	DrawHUD();
}
