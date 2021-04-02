#include "GameEntity.h"
#include "Drawer.h"
#include "C_Sprite.h"

GameEntity::GameEntity(const Vector2f& position) :
	position(position),
	isDeletedFlag(false) {}

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

Vector2f GameEntity::GetPosition() const { return position; }

Vector2f GameEntity::AddPosition(const Vector2f& toAdd)
{
	position += toAdd;
	return position;
}

void GameEntity::SetPosition(Vector2f position) 
{ 
	this->position = position; 
}

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
	if (sprite && sprite->isComponentOn)
	{
		sprite->Draw();
	}
}

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const
{
	return sprite;
}

void GameEntity::SetDelete() { isDeletedFlag = true; }

bool GameEntity::IsMarkedForDelete() const { return isDeletedFlag; }
