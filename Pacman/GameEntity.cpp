#include "GameEntity.h"
#include "Drawer.h"

GameEntity::GameEntity(const Vector2f& aPosition, const char* anImage)
:myPosition(aPosition)
,myImage(anImage)
,myIdMarkedForDeleteFlag(false)
{
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Start()
{
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{

	return false;	
}

void GameEntity::Draw(std::shared_ptr<Drawer> aDrawer)
{
	aDrawer->Draw(myImage, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}

const char* GameEntity::GetImage()
{
	return myImage;
}

std::shared_ptr<C_Sprite> GameEntity::GetSprite() const
{
	return mySprite;
}
