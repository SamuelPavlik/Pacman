#include "Avatar.h"
#include "Constants.h"
#include "C_KeyboardMovement.h"

Avatar::Avatar(const Vector2f& aPosition)
: MovableGameEntity(aPosition)
{

}

Avatar::~Avatar(void)
{
}

void Avatar::SetMovement(Input& input, const World& world)
{
	movement = std::make_shared<C_KeyboardMovement>(*this, input, world);
}

void Avatar::Update(float aTime)
{
	movement->Update(aTime);

	//Vector2f destination(myNextTileX * TILE_SIZE, myNextTileY * TILE_SIZE);
	//Vector2f direction = destination - myPosition;

	//float distanceToMove = aTime * AVATAR_SPEED;

	//if (distanceToMove > direction.Length())
	//{
	//	myPosition = destination;
	//	myCurrentTileX = myNextTileX;
	//	myCurrentTileY = myNextTileY;
	//}
	//else
	//{
	//	direction.Normalize();
	//	myPosition += direction * distanceToMove;
	//}
}
