#pragma once

#include "Component.h"
#include "Constants.h"
#include "Vector2f.h"

#include <memory>
#include <list>
#include <algorithm>
#include <functional>
#include <set>

class C_Animation;
class World;
class PathmapTile;
class GameEntity;

struct PathNode
{
    PathNode(const std::shared_ptr<const PathmapTile>& tile, std::shared_ptr<const PathNode> prev, int pathLength) :
        tile(tile),
        prev(prev),
        pathLength(pathLength) {}

    const std::shared_ptr<const PathmapTile> tile;
    std::shared_ptr<const PathNode> prev;
    int pathLength = 0;
};

using PathNodePtr = std::shared_ptr<const PathNode>;

class C_GhostBehavior : public Component
{
public:
    C_GhostBehavior(GameEntity& owner, const World& world, 
        const std::shared_ptr<const GameEntity>& avatar,
        float moveSpeed = GHOST_SPEED) noexcept;

	void Awake() override;
	void Start() override;
	void Update(float time) override;

    void Die();
    void MarkClaimable() noexcept;

protected:
    virtual Vector2f GetNextTile(const Vector2f& curr, bool isClaimableFlag) const = 0;

private:
    void Move(float time);
    void GetPath(int toX, int toY);
    
    PathNodePtr Pathfind(const std::shared_ptr<const PathmapTile>& fromTile,
        const std::shared_ptr<const PathmapTile>& toTile) const;

    bool Contains(std::set<std::shared_ptr<const PathmapTile>>& collection, 
        const std::shared_ptr<const PathmapTile>& fromTile) const;

public:
    bool isClaimableFlag;
    bool isDeadFlag;

protected:
    const std::shared_ptr<const GameEntity> avatar;
    const World& world;

private:
    float moveSpeed;
    std::shared_ptr<C_Animation> animation;

    std::list<std::shared_ptr<const PathmapTile>> path;
    int currentTileX;
    int currentTileY;
    int nextTileX;
    int nextTileY;
    float claimableCounter;
    float releaseCounter;
};

