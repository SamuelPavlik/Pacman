#include "GameEntity.h"
#include "Drawer.h"
#include "C_Sprite.h"

GameEntity::GameEntity(const Vector2f& aPosition)
:myPosition(aPosition)
,isDeletedFlag(false) {}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Awake()
{
	isDeletedFlag = false;
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Awake();
	}
}

void GameEntity::Start()
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Start();
	}
}

Vector2f GameEntity::AddPosition(const Vector2f& toAdd)
{
	myPosition += toAdd;
	return myPosition;
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{

	return false;	
}

void GameEntity::Update(float time)
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Update(time);
	}
}

void GameEntity::Draw()
{
	if (mySprite)
	{
		mySprite->Draw();
	}
}

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const
{
	return mySprite;
}
