#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "MovableGameEntity.h"

class World;
class PathmapTile;

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition);
	~Ghost(void);

	void Update(float aTime) override;

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetWorld(World& world);

	void Die();

protected:

	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<std::shared_ptr<PathmapTile>> myPath;

private:
	World* world;
};

#endif // GHOST_H