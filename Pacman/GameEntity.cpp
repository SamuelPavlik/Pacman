#include "GameEntity.h"
#include "Drawer.h"
#include "C_Sprite.h"

#include <algorithm>

GameEntity::GameEntity(const Vector2f& position) :
	position(position),
	isDeletedFlag(false) {}

void GameEntity::Awake()
{
	isDeletedFlag = false;
	std::for_each(components.rbegin(), components.rend(), [](auto& comp) {
		comp->Awake();
		});
}

void GameEntity::Start()
{
	std::for_each(components.rbegin(), components.rend(), [](auto& comp) {
		comp->Start();
		});
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
	std::for_each(components.rbegin(), components.rend(), [time](auto& comp) {
		if (comp->isComponentOn)
			comp->Update(time);
		});
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
