#include "C_GhostBehavior.h"
#include "C_Animation.h"
#include "GameEntity.h"
#include "World.h"
#include "PathmapTile.h"

#include <set>
#include <queue>

struct PathNode
{
	PathNode(std::shared_ptr<PathmapTile> tile, float pathLength) :
		tile(tile),
		pathLength(pathLength) {}

	std::shared_ptr<PathmapTile> tile;
	float pathLength = 0.f;
};

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

void C_GhostBehavior::GetPath(int aToX, int aToY)
{
	const auto map = world->GetMap();
	auto fromTile = map[currentTileX][currentTileY];
	auto toTile = map[aToY][aToX];

	for (const auto line : map)
	{
		for (const auto tile : line)
		{
			tile->myIsVisitedFlag = false;
		}
	}

	Pathfind(fromTile, toTile);
}

bool C_GhostBehavior::Pathfind(std::shared_ptr<PathmapTile> aFromTile, 
	std::shared_ptr<PathmapTile> aToTile)
{
	if (aFromTile->myIsBlockingFlag)
		return false;

	const auto map = world->GetMap();
	std::set<std::shared_ptr<PathmapTile>> visited;
	//compare function for priority queue
	auto comp = [aToTile](auto a, auto b) {
		int la = abs(a.tile->myX - aToTile->myX) + abs(a.tile->myY - aToTile->myY);
		int lb = abs(b.tile->myX - aToTile->myX) + abs(b.tile->myY - aToTile->myY);
		return la < lb;
	};
	std::priority_queue<PathNode, std::vector<PathNode>, decltype(comp)> tileQueue;

	tileQueue.push(PathNode(aFromTile, 0.f));
	while (!tileQueue.empty())
	{
		auto next = tileQueue.top(); tileQueue.pop();

		if (next.tile == aToTile)
			return true;

		//init and populate a collection of adjacent tiles
		std::vector<std::shared_ptr<PathmapTile>> neighborList (4);

		auto up = map[aFromTile->myY - 1][aFromTile->myX];
		if (up && !Contains(visited, up) && !up->myIsBlockingFlag && !Contains(path, up))
			tileQueue.push(up, tile);

		auto down = map[aFromTile->myY + 1][aFromTile->myX];
		if (down && !Contains(visited, down) && !down->myIsBlockingFlag && !Contains(path, down))
			tileQueue.push(down);

		auto right = map[aFromTile->myY][aFromTile->myX + 1];
		if (right && !Contains(visited, right) && !right->myIsBlockingFlag && !Contains(path, right))
			tileQueue.push(right);

		auto left = map[aFromTile->myY][aFromTile->myX - 1];
		if (left && !Contains(visited, left) && !left->myIsBlockingFlag && !Contains(path, left))
			tileQueue.push(left);

		//sort the adjacent tiles by euclidian distance
		//std::sort(neighborList.begin(), neighborList.end(), comp);

		//find the shortest path recursively
		for (auto tile : neighborList)
		{
			path.push_back(tile);

			if (Pathfind(tile, aToTile))
				return true;

			path.pop_back();
		}
	}


	return false;
}
