#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "C_Sprite.h"

Ghost::Ghost(const Vector2f& aPosition)
: GameEntity(aPosition) {}

Ghost::~Ghost(void) {}

void Ghost::SetWorld(World* world)
{
	this->world = world;
}

void Ghost::Die()
{
	myPath.clear();
	//world->GetPath(myCurrentTileX, myCurrentTileY, 13, 13, myPath);
	//TODO temporary solution; try to actually get a path
	SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE, GHOST_START_TILE_Y * TILE_SIZE));
	Start();
}

void Ghost::Update(float aTime)
{
	float speed = 30.f;
	int nextTileX = myCurrentTileX + myDesiredMovementX;
	int nextTileY = myCurrentTileY + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

	if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
	{
		if (!myPath.empty())
		{
			auto nextTile = myPath.front();
			myPath.pop_front();
			myNextTileX = nextTile->myX;
			myNextTileY = nextTile->myY;
		}
		else if (world->TileIsValid(nextTileX, nextTileY))
		{
			myNextTileX = nextTileX;
			myNextTileY = nextTileY;
		}
		else
		{
			if (myDesiredMovementX == 1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = 1;
			} else if (myDesiredMovementY == 1)
			{
				myDesiredMovementX = -1;
				myDesiredMovementY = 0;			
			} else if (myDesiredMovementX == -1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = -1;
			} else
			{
				myDesiredMovementX = 1;
				myDesiredMovementY = 0;
			}

			myIsDeadFlag = false;
		}
	}

	int tileSize = 22;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

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

	//drawing logic
	if (myIsDeadFlag)
		mySprite->SetName("Ghost_Dead_32.png");
	else if (myIsClaimableFlag)
		mySprite->SetName("Ghost_Vulnerable_32.png");
}

void Ghost::Start()
{
	mySprite->SetName("ghost_32.png");
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
	myCurrentTileX = myNextTileX = myPosition.myX / TILE_SIZE;
	myCurrentTileY = myNextTileY = myPosition.myY / TILE_SIZE;
}
