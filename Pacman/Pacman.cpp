#include "Pacman.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"
#include "C_GhostBehavior.h"
#include "EntityFactory.h"

#include <sstream>
#include <string>

std::shared_ptr<Pacman> Pacman::Create(Drawer& drawer)
{
	std::shared_ptr<Pacman> pacman = std::make_shared<Pacman>(drawer);
	pacman->Init();
	return pacman;
}

Pacman::Pacman(Drawer& drawer) :
	drawer(drawer),
	score(0),
	bestScore(0),
	fps(0),
	lives(3),
	isGameOver(false),
	menuState(MenuState::None),
	gameOverText(""),
	world(),
	gameEndCounter(0.f),
	totalPoints() {}

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
	EntityFactory factory(drawer, inputManager, world);

	//set up world
	world.Init(drawer, factory, dots, bigDots);

	//set up avatar
	std::function<void(CollisionData)> onOverlapFunc = [this](CollisionData cd) {
		OnIntersectedDot(cd);
		OnIntersectedBigDot(cd);
		OnAvatarGhostCollision(cd);
	};
	avatar = factory.CreatePacman(
		Vector2f(PACMAN_START_TILE_X * TILE_SIZE, PACMAN_START_TILE_Y * TILE_SIZE), 
		onOverlapFunc, "open_32.png");
	
	//set up ghost
	ghosts.push_back(factory.CreateGhost(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE), Vector2f(), avatar, "ghost_32.png"));
	ghosts.push_back(factory.CreateGhost(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE), Vector2f(4.f, 4.f), avatar, "ghost_32.png"));
	
	//add all entities to entity collection
	Restart();
}

void Pacman::Restart()
{
	avatar->SetPosition(Vector2f(PACMAN_START_TILE_X * TILE_SIZE,
		PACMAN_START_TILE_Y * TILE_SIZE));
	ghosts[0]->SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE));
	ghosts[1]->SetPosition(Vector2f((GHOST_START_TILE_X + 1) * TILE_SIZE,
		(GHOST_START_TILE_Y + 1) * TILE_SIZE));

	entityManager.Clear();
	entityManager.Add(dots);
	entityManager.Add(bigDots);
	entityManager.Add(ghosts);
	entityManager.Add(avatar);

	totalPoints = dots.size();
	lives = 3;
	bestScore = std::max(score, bestScore);
	score = 0;

	soundManager.Play(MENU_SOUND);
	menuState = MenuState::None;
}

bool Pacman::Update(float time)
{
	if (menuState == MenuState::None)
	{
		menuState = menu.Update(inputManager);
		if (menuState == MenuState::None)
		{
			return true;
		}
		else if (menuState == MenuState::Quit)
		{
			return false;
		}
		else
		{
			soundManager.StopAll();
		}
	}

	entityManager.ProcessRemovals();
	entityManager.ProcessNewEntities();

	if (inputManager.IsKeyPressed(InputManager::Key::Esc))
	{
		return false;
	}

	entityManager.Update(time);

	CheckEndGameCondition(time);

	if (time > 0)
	{
		fps = (int) (1 / time);
	}

	return true;
}

void Pacman::OnIntersectedDot(CollisionData cd)
{
	if (cd.other->tag != DOT_TAG)
	{
		return;
	}

	soundManager.Play(DOT_SOUND);
	score += SMALL_DOT_POINTS;
	cd.other->SetDelete();
	totalPoints--;
}

void Pacman::OnIntersectedBigDot(CollisionData cd)
{
	if (cd.other->tag != BIG_DOT_TAG)
	{
		return;
	}

	soundManager.Play(BIG_DOT_SOUND);

	//game logic when dot eaten
	score += BIG_DOT_POINTS;
	for (auto ghost : ghosts)
	{
		if (auto moveComp = ghost->GetComponent<C_GhostBehavior>())
		{
			moveComp->MarkClaimable();
		}
	}
		
	//delete dot
	cd.other->SetDelete();
}

void Pacman::OnAvatarGhostCollision(CollisionData cd)
{
	std::shared_ptr<C_GhostBehavior> moveComp;
	if (cd.other->tag != ENEMY_TAG || !(moveComp = cd.other->GetComponent<C_GhostBehavior>()))
	{
		return;
	}

	if (moveComp->isDeadFlag)
	{
		return;
	}

	if (!moveComp->isClaimableFlag)
	{
		lives--;
		//reset avatar
		avatar->SetPosition(Vector2f(PACMAN_START_TILE_X * TILE_SIZE,
			PACMAN_START_TILE_Y * TILE_SIZE));
		avatar->Start();
	}
	else
	{
		soundManager.Play(EAT_GHOST_SOUND);
		moveComp->Die();
		score += 50;
	}
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
	bool isLost = lives <= 0;
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
			avatar->SetDelete();
			gameOverText = "You lose!";
			soundManager.Play(PAC_DEATH_SOUND);
		}
	}
}

void Pacman::DrawHUD()
{
	std::stringstream scoreStream;
	scoreStream << score;
	drawer.DrawText("Score", HUD_FONT, SCORE_X, SCORE_Y);
	drawer.DrawText(scoreStream.str().c_str(), HUD_FONT, SCORE_NUM_X, SCORE_Y);

	std::stringstream liveStream;
	liveStream << lives;
	drawer.DrawText("Lives", HUD_FONT, LIVES_X, LIVES_Y);
	drawer.DrawText(liveStream.str().c_str(), HUD_FONT, LIVES_NUM_X, LIVES_Y);

	std::stringstream bestStream;
	bestStream << bestScore;
	drawer.DrawText("Best", HUD_FONT, BEST_SCORE_X, BEST_SCORE_Y);
	drawer.DrawText(bestStream.str().c_str(), HUD_FONT, BEST_SCORE_NUM_X, BEST_SCORE_Y);

	std::stringstream fpsStream;
	fpsStream << fps;
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
	world.Draw(drawer);
	if (menuState == MenuState::None)
	{
		menu.Draw(drawer);
		return;
	}

	entityManager.Draw();
	DrawHUD();
}
