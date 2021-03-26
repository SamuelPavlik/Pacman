#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "GameEntity.h"

class World;
class PathmapTile;

class Ghost : public GameEntity
{
public:
	Ghost(const Vector2f& aPosition);
	~Ghost(void);

	void Update(float aTime) override;
	void Start() override;

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetWorld(World* world);

	void Die();

protected:

	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<std::shared_ptr<PathmapTile>> myPath;

private:
	int myCurrentTileX;
	int myCurrentTileY;
	int myNextTileX;
	int myNextTileY;

	World* world;
};

#endif // GHOST_H