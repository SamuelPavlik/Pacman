#include "MovableGameEntity.h"
#include "Constants.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition)
: GameEntity(aPosition)
{
	myCurrentTileX = myNextTileX = myPosition.myX / TILE_SIZE;
	myCurrentTileY = myNextTileY =  myPosition.myY / TILE_SIZE;
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtDestination()
{
	if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
	{


		return true;
	}

	return false;
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}
