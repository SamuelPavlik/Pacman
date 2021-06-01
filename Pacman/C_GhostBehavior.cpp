#include "C_GhostBehavior.h"
#include "C_Animation.h"
#include "GameEntity.h"
#include "World.h"
#include "PathmapTile.h"

#include <queue>

C_GhostBehavior::C_GhostBehavior(GameEntity& owner, const World& world,
	const std::shared_ptr<const GameEntity>& avatar, float moveSpeed) noexcept :
	Component(owner),
	isClaimableFlag(false),
	isDeadFlag(false),
	world(world),
	avatar(avatar),
	moveSpeed(moveSpeed),
	currentTileX(),
	currentTileY(),
	nextTileX(),
	nextTileY(),
	claimableCounter(),
	releaseCounter() {}

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
	currentTileX = nextTileX = owner.GetPosition().x / TILE_SIZE;
	currentTileY = nextTileY = owner.GetPosition().y / TILE_SIZE;
	moveSpeed = GHOST_SPEED;
	claimableCounter = GHOST_COUNTER;
	releaseCounter = RELEASE_COUNTER;
}


void C_GhostBehavior::Update(float time)
{
	//movement logic
	if (releaseCounter < 0.f)
	{
		Move(time);
	}
	else
	{
		releaseCounter -= time;
	}

	//animation logic
	if (isDeadFlag)
	{
		animation->SetAnimationState(AnimationState::Dead);
	}
	else if (isClaimableFlag)
	{
		animation->SetAnimationState(AnimationState::Vulnerable);
	}
	else
	{
		animation->SetAnimationState(AnimationState::GoingUp);
	}

	//claimable logic
	if (claimableCounter > 0.f)
	{
		claimableCounter -= time;
	}
	else
	{
		isClaimableFlag = false;
	}
}

void C_GhostBehavior::Die()
{
	isDeadFlag = true;
	isClaimableFlag = false;
	moveSpeed = DEATH_SPEED_MULTIPLIER * moveSpeed;
	GetPath(GHOST_START_TILE_X, GHOST_START_TILE_Y);
}

void C_GhostBehavior::MarkClaimable() noexcept
{
	isClaimableFlag = true;
	claimableCounter = GHOST_COUNTER;
}

void C_GhostBehavior::Move(float time)
{
	//calculate next tile when goal tile reached
	if (currentTileX == nextTileX && currentTileY == nextTileY)
	{
		if (!isDeadFlag && !avatar->IsMarkedForDelete())
		{
			auto pos = GetNextTile(Vector2f(currentTileX, currentTileY), isClaimableFlag);
			GetPath(pos.x, pos.y);
		}

		if (!path.empty())
		{
			auto nextTile = path.front();
			path.pop_front();

			if (path.empty() && isDeadFlag) 
			{
				Start();
			}

			nextTileX = nextTile->x;
			nextTileY = nextTile->y;
		}
	}

	//set position based on goal tile
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

void C_GhostBehavior::GetPath(int toX, int toY)
{
	path.clear();
	const auto& map = world.GetMap();
	auto fromTile = map[currentTileY][currentTileX];
	auto toTile = map[toY][toX];
	auto foundPath = Pathfind(fromTile, toTile);

	while (foundPath && foundPath->prev) 
	{
		path.push_front(foundPath->tile);
		foundPath = foundPath->prev;
	}

	nextTileX = currentTileX;
	nextTileY = currentTileY;
}

auto GetCompareFunction(bool isClaimableFlag, const std::shared_ptr<const PathmapTile>& toTile) {
	std::function<bool(PathNodePtr, PathNodePtr)> comp;
	if (isClaimableFlag) 
	{
		comp = [&toTile](const auto& a, const auto& b) {
			int la = a->pathLength + abs(a->tile->x - toTile->x) + abs(a->tile->y - toTile->y);
			int lb = b->pathLength + abs(b->tile->x - toTile->x) + abs(b->tile->y - toTile->y);
			return la < lb;
		};
	}
	else 
	{
		comp = [&toTile](const auto& a, const auto& b) {
			int la = a->pathLength + abs(a->tile->x - toTile->x) + abs(a->tile->y - toTile->y);
			int lb = b->pathLength + abs(b->tile->x - toTile->x) + abs(b->tile->y - toTile->y);
			return la > lb;
		};
	}

	return comp;
}

PathNodePtr C_GhostBehavior::Pathfind(const std::shared_ptr<const PathmapTile>& fromTile,
	const std::shared_ptr<const PathmapTile>& toTile) const
{
	if (fromTile->isBlockingFlag)
	{
		return false;
	}

	const auto& map = world.GetMap();
	std::set<std::shared_ptr<const PathmapTile>> visited;

	//compare function for priority queue
	//changes when ghost is claimable
	auto comp = GetCompareFunction(isClaimableFlag, toTile);
	std::priority_queue<PathNodePtr, std::vector<PathNodePtr>, decltype(comp)> tileQueue (comp);

	//A* search to find player up to limited depth of the tree
	tileQueue.emplace(std::make_shared<PathNode>(fromTile, nullptr, 0.f));
	visited.insert(fromTile);
	while (!tileQueue.empty())
	{
		auto prev = tileQueue.top(); tileQueue.pop();

		//search with limited depth when looking for player
		if (prev->tile == toTile || (!isDeadFlag && prev->pathLength >= MAX_PATH_SEARCH_LENGTH))
		{
			return PathNodePtr(prev);
		}

		auto up = map[prev->tile->y - 1][prev->tile->x];
		if (up && !Contains(visited, up) && !up->isBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(up, prev, prev->pathLength + 1));
			visited.insert(up);
		}

		auto down = map[prev->tile->y + 1][prev->tile->x];
		if (down && !Contains(visited, down) && !down->isBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(down, prev, prev->pathLength + 1));
			visited.insert(down);
		}

		auto right = map[prev->tile->y][prev->tile->x + 1];
		if (right && !Contains(visited, right) && !right->isBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(right, prev, prev->pathLength + 1));
			visited.insert(right);
		}

		auto left = map[prev->tile->y][prev->tile->x - 1];
		if (left && !Contains(visited, left) && !left->isBlockingFlag)
		{
			tileQueue.emplace(std::make_shared<PathNode>(left, prev, prev->pathLength + 1));
			visited.insert(left);
		}
	}

	return nullptr;
}

bool C_GhostBehavior::Contains(std::set<std::shared_ptr<const PathmapTile>>& collection, 
	const std::shared_ptr<const PathmapTile>& fromTile) const     {
	return std::find(collection.begin(), collection.end(), fromTile) != collection.end();
}
