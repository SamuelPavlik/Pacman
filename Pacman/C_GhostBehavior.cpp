#include "C_GhostBehavior.h"
#include "C_Animation.h"
#include "GameEntity.h"
#include "World.h"
#include "PathmapTile.h"

#include <set>
#include <queue>
#include <chrono>

C_GhostBehavior::C_GhostBehavior(GameEntity& owner, const World* world, 
	std::shared_ptr<GameEntity> avatar, float moveSpeed) :
	Component(owner),
	world(world),
	avatar(avatar),
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
		if (!isDeadFlag)
		{
			auto pos = avatar->GetPosition();
			GetPath(pos.myX / TILE_SIZE, pos.myY / TILE_SIZE);
		}

		if (!path.empty())
		{
			auto nextTile = path.front();
			path.pop_front();

			if (path.empty() && isDeadFlag)
				Start();

			nextTileX = nextTile->myX;
			nextTileY = nextTile->myY;
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

	while (foundPath && foundPath->prev) 
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
	visited.insert(aFromTile);
	while (!tileQueue.empty())
	{
		auto prev = tileQueue.top(); tileQueue.pop();

		//search with limited depth when looking for player
		if (prev->tile == aToTile || (!isDeadFlag && prev->pathLength >= MAX_PATH_SEARCH_LENGTH))
		{
			return PathNodePtr(prev);
		}

		auto up = map[prev->tile->myY - 1][prev->tile->myX];
		if (up && !Contains(visited, up) && !up->myIsBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(up, prev, prev->pathLength + 1));
			visited.insert(up);
		}

		auto down = map[prev->tile->myY + 1][prev->tile->myX];
		if (down && !Contains(visited, down) && !down->myIsBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(down, prev, prev->pathLength + 1));
			visited.insert(down);
		}

		auto right = map[prev->tile->myY][prev->tile->myX + 1];
		if (right && !Contains(visited, right) && !right->myIsBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(right, prev, prev->pathLength + 1));
			visited.insert(right);
		}

		auto left = map[prev->tile->myY][prev->tile->myX - 1];
		if (left && !Contains(visited, left) && !left->myIsBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(left, prev, prev->pathLength + 1));
			visited.insert(left);
		}
	}

	return NULL;
}
