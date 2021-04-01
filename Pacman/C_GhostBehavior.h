#pragma once

#include "Component.h"
#include "Constants.h"

#include <memory>
#include <list>
#include <algorithm>

class C_Animation;
class World;
class PathmapTile;
class GameEntity;

struct PathNode
{
    PathNode(std::shared_ptr<PathmapTile> tile, std::shared_ptr<PathNode> prev, int pathLength) :
        tile(tile),
        prev(prev),
        pathLength(pathLength) {}

    std::shared_ptr<PathmapTile> tile;
    std::shared_ptr<PathNode> prev;
    int pathLength = 0;
};

using PathNodePtr = std::shared_ptr<PathNode>;

class C_GhostBehavior : public Component
{
public:
    C_GhostBehavior(GameEntity& owner, const World* world, 
        std::shared_ptr<GameEntity> avatar, float moveSpeed = GHOST_SPEED);

	void Awake() override;
	void Start() override;
	void Update(float time) override;

    void Die();
    void MarkClaimable();

private:
    void Move(float time);
    void GetPath(int aToX, int aToY);
    PathNodePtr Pathfind(std::shared_ptr<PathmapTile> aFromTile,
        std::shared_ptr<PathmapTile> aToTile);

    template<typename T>
    bool Contains(T collection, std::shared_ptr<PathmapTile> aFromTile)
    {
        return std::find(collection.begin(), collection.end(), aFromTile) != collection.end();
    }


public:
    bool isClaimableFlag;
    bool isDeadFlag;

private:
    float moveSpeed;
    const World* world;
    std::shared_ptr<C_Animation> animation;
    std::shared_ptr<GameEntity> avatar;

    std::list<std::shared_ptr<PathmapTile>> path;
    int desiredMovementX;
    int desiredMovementY;
    int currentTileX;
    int currentTileY;
    int nextTileX;
    int nextTileY;
    float claimableCounter;
};

