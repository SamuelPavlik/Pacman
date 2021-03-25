#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2f& aPosition);
	~MovableGameEntity(void);

	void SetNextTile(int anX, int anY);
	int GetCurrentTileX() const { return myCurrentTileX; }
	int GetCurrentTileY() const { return myCurrentTileY; }

	bool IsAtDestination();

protected:

	int myCurrentTileX;
	int myCurrentTileY;

	int myNextTileX;
	int myNextTileY;

};

#endif // MOVABLEGAMEENTITY_H