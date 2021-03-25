#include "GameEntity.h"
#include "Drawer.h"
#include "C_Sprite.h"

GameEntity::GameEntity(const Vector2f& aPosition)
:myPosition(aPosition)
,myIdMarkedForDeleteFlag(false) {}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Start()
{
}

void GameEntity::SetSprite(std::shared_ptr<Drawer> drawer, const char* name)
{
	mySprite = std::make_shared<C_Sprite>(*this, drawer, name);
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{

	return false;	
}

void GameEntity::Update(float time)
{
}

void GameEntity::Draw(std::shared_ptr<Drawer> drawer)
{
	if (mySprite)
	{
		mySprite->Draw(drawer);
	}
}

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const
{
	return mySprite;
}
