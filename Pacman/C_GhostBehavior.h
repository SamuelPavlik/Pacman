#pragma once

#include "Component.h"
#include "Constants.h"

#include <memory>
#include <list>
#include <algorithm>

class C_Animation;
class World;
class PathmapTile;

class C_GhostBehavior : public Component
{
public:
    C_GhostBehavior(GameEntity& owner, const World* world, float moveSpeed = GHOST_SPEED);

	void Awake() override;
	void Start() override;
	void Update(float time) override;

    void Die();

private:
    void Move(float time);
    void GetPath(int aToX, int aToY);
    bool Pathfind(std::shared_ptr<PathmapTile> aFromTile, 
        std::shared_ptr<PathmapTile> aToTile);

    template<typename T>
    bool Contains(T collection, std::shared_ptr<PathmapTile> aFromTile)
    {
        return std::find(collection.begin(), collection.end(), aFromTile);
    }


public:
    bool isClaimableFlag;
    bool isDeadFlag;

private:
    float moveSpeed;
    const World* world;
    std::shared_ptr<C_Animation> animation;

    std::list<std::shared_ptr<PathmapTile>> path;
    int desiredMovementX;
    int desiredMovementY;
    int currentTileX;
    int currentTileY;
    int nextTileX;
    int nextTileY;
};

