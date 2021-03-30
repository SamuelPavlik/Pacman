#include "C_GhostBehavior.h"
#include "C_Animation.h"
#include "GameEntity.h"
#include "World.h"
#include "PathmapTile.h"

#include <set>
#include <queue>

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
	isDeadFlag = true;
	moveSpeed = 5 * moveSpeed;
	GetPath(GHOST_START_TILE_X, GHOST_START_TILE_Y);
}

void C_GhostBehavior::Move(float time)
{
	int possibleTileX = currentTileX + desiredMovementX;
	int possibleTileY = currentTileY + desiredMovementY;

	if (currentTileX == nextTileX && currentTileY == nextTileY)
	{
		if (!path.empty())
		{
			auto nextTile = path.front();
			path.pop_front();

			if (path.empty())
				Start();

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

void C_GhostBehavior::GetPath(int aToX, int aToY)
{
	path.clear();
	const auto map = world->GetMap();
	auto fromTile = map[currentTileY][currentTileX];
	auto toTile = map[aToY][aToX];

	auto foundPath = Pathfind(fromTile, toTile);
	while (foundPath) 
	{
		auto nodeTile = foundPath->tile;
		path.push_front(nodeTile);
		foundPath = foundPath->prev;
	}

	nextTileX = currentTileX;
	nextTileY = currentTileY;
}

PathNodePtr C_GhostBehavior::Pathfind(std::shared_ptr<PathmapTile> aFromTile,
	std::shared_ptr<PathmapTile> aToTile)
{
	if (aFromTile->myIsBlockingFlag)
		return false;

	const auto map = world->GetMap();
	std::set<std::shared_ptr<PathmapTile>> visited;
	//compare function for priority queue
	auto comp = [aToTile](auto a, auto b) {
		int la = a->pathLength + abs(a->tile->myX - aToTile->myX) + abs(a->tile->myY - aToTile->myY);
		int lb = b->pathLength + abs(b->tile->myX - aToTile->myX) + abs(b->tile->myY - aToTile->myY);
		return la > lb;
	};
	std::priority_queue<PathNodePtr, std::vector<PathNodePtr>, decltype(comp)> tileQueue (comp);

	tileQueue.emplace(std::make_shared<PathNode>(PathNode(aFromTile, NULL, 0.f)));
	while (!tileQueue.empty())
	{
		auto prev = tileQueue.top(); tileQueue.pop();
		visited.insert(prev->tile);
		if (prev->tile == aToTile)
			return PathNodePtr(prev);

		auto up = map[prev->tile->myY - 1][prev->tile->myX];
		if (up && !Contains(visited, up) && !up->myIsBlockingFlag)
			tileQueue.emplace(std::make_shared<PathNode>(up, prev, prev->pathLength + TILE_SIZE));

		auto down = map[prev->tile->myY + 1][prev->tile->myX];
		if (down && !Contains(visited, down) && !down->myIsBlockingFlag)
			tileQueue.emplace(std::make_shared<PathNode>(down, prev, prev->pathLength + TILE_SIZE));

		auto right = map[prev->tile->myY][prev->tile->myX + 1];
		if (right && !Contains(visited, right) && !right->myIsBlockingFlag)
			tileQueue.emplace(std::make_shared<PathNode>(right, prev, prev->pathLength + TILE_SIZE));

		auto left = map[prev->tile->myY][prev->tile->myX - 1];
		if (left && !Contains(visited, left) && !left->myIsBlockingFlag)
			tileQueue.emplace(std::make_shared<PathNode>(left, prev, prev->pathLength + TILE_SIZE));
	}


	return NULL;
}
