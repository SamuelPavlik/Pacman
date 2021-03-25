#include "Avatar.h"
#include "Constants.h"

Avatar::Avatar(const Vector2f& aPosition)
: MovableGameEntity(aPosition)
{

}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	Vector2f destination(myNextTileX * TILE_SIZE, myNextTileY * TILE_SIZE);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * AVATAR_SPEED;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}
