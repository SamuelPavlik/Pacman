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

Vector2f GameEntity::GetPosition() const noexcept { return position; }

Vector2f GameEntity::AddPosition(const Vector2f& toAdd) noexcept
{
	position += toAdd;
	return position;
}

void GameEntity::SetPosition(const Vector2f& position) noexcept 
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

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const noexcept
{
	return sprite;
}

void GameEntity::SetDelete() noexcept { isDeletedFlag = true; }

bool GameEntity::IsMarkedForDelete() const noexcept { return isDeletedFlag; }
