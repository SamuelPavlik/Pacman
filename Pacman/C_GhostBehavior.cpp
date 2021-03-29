#include "C_GhostBehavior.h"
#include "C_Animation.h"
#include "GameEntity.h"
#include "World.h"
#include "PathmapTile.h"

C_GhostBehavior::C_GhostBehavior(GameEntity& owner, const World* world, float moveSpeed) :
	Component(owner),
	world(world),
	moveSpeed(moveSpeed) {}

void C_GhostBehavior::Awake()
{
	animation = owner.GetComponent<C_Animation>();
}

void C_GhostBehavior::Start()
{
	animation->SetAnimationState(AnimationState::GoingUp);
	path.clear();
	isClaimableFlag = false;
	isDeadFlag = false;
	desiredMovementX = 0;
	desiredMovementY = -1;
	currentTileX = nextTileX = owner.GetPosition().myX / TILE_SIZE;
	currentTileY = nextTileY = owner.GetPosition().myY / TILE_SIZE;
	moveSpeed = GHOST_SPEED;
}

void C_GhostBehavior::Update(float time)
{
	Move(time);

	//animation logic
	if (isDeadFlag)
		animation->SetAnimationState(AnimationState::Dead);
	else if (isClaimableFlag)
		animation->SetAnimationState(AnimationState::Vulnerable);
	else
		animation->SetAnimationState(AnimationState::GoingUp);
}

void C_GhostBehavior::Die()
{
	owner.SetPosition(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE));
	Start();
}

void C_GhostBehavior::Move(float time)
{
	int possibleTileX = currentTileX + desiredMovementX;
	int possibleTileY = currentTileY + desiredMovementY;

	if (isDeadFlag)
		moveSpeed = 4 * moveSpeed;

	if (currentTileX == nextTileX && currentTileY == nextTileY)
	{
		if (!path.empty())
		{
			auto nextTile = path.front();
			path.pop_front();
			nextTileX = nextTile->myX;
			nextTileY = nextTile->myY;
		}
		else if (world->TileIsValid(possibleTileX, possibleTileY))
		{
			nextTileX = possibleTileX;
			nextTileY = possibleTileY;
		}
		else
		{
			if (desiredMovementX == 1)
			{
				desiredMovementX = 0;
				desiredMovementY = 1;
			}
			else if (desiredMovementY == 1)
			{
				desiredMovementX = -1;
				desiredMovementY = 0;
			}
			else if (desiredMovementX == -1)
			{
				desiredMovementX = 0;
				desiredMovementY = -1;
			}
			else
			{
				desiredMovementX = 1;
				desiredMovementY = 0;
			}
		}
	}

	Vector2f destination(nextTileX * TILE_SIZE, nextTileY * TILE_SIZE);
	Vector2f direction = destination - owner.GetPosition();

	float distanceToMove = time * moveSpeed;

	if (distanceToMove > direction.Length())
	{
		owner.SetPosition(destination);
		currentTileX = nextTileX;
		currentTileY = nextTileY;
	}
	else
	{
		direction.Normalize();
		owner.AddPosition(direction * distanceToMove);
	}
}
