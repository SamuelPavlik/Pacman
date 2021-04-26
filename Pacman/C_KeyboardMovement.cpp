#include "C_KeyboardMovement.h"
#include "C_Animation.h"
#include "InputManager.h"
#include "World.h"
#include "GameEntity.h"

C_KeyboardMovement::C_KeyboardMovement(GameEntity& owner, InputManager& inputManager, 
	const World& world, float moveSpeed) : 
	Component(owner), 
	moveSpeed(moveSpeed),
	inputManager(inputManager),
	world(world),
	currentTileX(),
	currentTileY(),
	nextTileX(),
	nextTileY()
{}

void C_KeyboardMovement::SetMoveSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

const Vector2f C_KeyboardMovement::GetDirection() const
{
	return nextMovement;
}

void C_KeyboardMovement::Awake()
{
	animation = owner.GetComponent<C_Animation>();
}

void C_KeyboardMovement::Update(float time)
{
	Vector2f possibleMovement;
	if (inputManager.IsKeyPressed(InputManager::Key::Up))
	{
		possibleMovement = Vector2f(0.f, -1.f);
	}
	else if (inputManager.IsKeyPressed(InputManager::Key::Down))
	{
		possibleMovement = Vector2f(0.f, 1.f);
	}
	else if (inputManager.IsKeyPressed(InputManager::Key::Left))
	{
		possibleMovement = Vector2f(-1.f, 0.f);
	}
	else if (inputManager.IsKeyPressed(InputManager::Key::Right))
	{
		possibleMovement = Vector2f(1.f, 0.f);
	}

	if (possibleMovement == Vector2f(0.f, 0.f))
	{
		possibleMovement = nextMovement;
	}
	Move(time, possibleMovement);
}

void C_KeyboardMovement::Start()
{
	nextMovement = Vector2f(-1.f, 0.f);
	currentTileX = nextTileX = owner.GetPosition().x / TILE_SIZE;
	currentTileY = nextTileY = owner.GetPosition().y / TILE_SIZE;
}

void C_KeyboardMovement::Move(float time, Vector2f possibleMove)
{
	auto possibleTileX = currentTileX + possibleMove.x;
	auto possibleTileY = currentTileY + possibleMove.y;

	//calculate next tile when goal tile reached
	if (currentTileX == nextTileX && currentTileY == nextTileY
		|| possibleMove != nextMovement)
	{
		if (world.TileIsValid(possibleTileX, possibleTileY))
		{
			nextTileX = possibleTileX;
			nextTileY = possibleTileY;
			nextMovement = possibleMove;
		}
		else if (world.TileIsValid(currentTileX + nextMovement.x, currentTileY + nextMovement.y))
		{
			nextTileX = currentTileX + nextMovement.x;
			nextTileY = currentTileY + nextMovement.y;
		}

		//set animation based on direction
		auto state = animation->GetAnimationState();
		if (nextMovement.x == 1)
		{
			if (animation && state != AnimationState::GoingRight)
				animation->SetAnimationState(AnimationState::GoingRight);
		}
		else if (nextMovement.x == -1)
		{
			if (animation && state != AnimationState::GoingLeft)
				animation->SetAnimationState(AnimationState::GoingLeft);
		}
		else if (nextMovement.y == 1)
		{
			if (animation && state != AnimationState::GoingDown)
				animation->SetAnimationState(AnimationState::GoingDown);
		}
		else if (nextMovement.y == -1)
		{
			if (animation && state != AnimationState::GoingUp)
				animation->SetAnimationState(AnimationState::GoingUp);
		}
	}

	//set position based on goal tile
	Vector2f destination(nextTileX * TILE_SIZE, nextTileY * TILE_SIZE);
	auto direction = destination - owner.GetPosition();
	auto distanceToMove = time * moveSpeed;

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
