#include "GameEntity.h"
#include "Drawer.h"
#include "C_Sprite.h"

GameEntity::GameEntity(const Vector2f& aPosition)
:myPosition(aPosition)
,isDeletedFlag(false) {}

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

Vector2f GameEntity::GetPosition() const { return myPosition; }

Vector2f GameEntity::AddPosition(const Vector2f& toAdd)
{
	myPosition += toAdd;
	return myPosition;
}

void GameEntity::SetPosition(const Vector2f& aPosition) { myPosition = aPosition; }

void GameEntity::Update(float time)
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		if (components[i]->isComponentOn)
			components[i]->Update(time);
	}
}

void GameEntity::Draw()
{
	if (mySprite && mySprite->isComponentOn)
	{
		mySprite->Draw();
	}
}

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const
{
	return mySprite;
}

void GameEntity::SetDelete() { isDeletedFlag = true; }

bool GameEntity::IsMarkedForDelete() const { return isDeletedFlag; }
