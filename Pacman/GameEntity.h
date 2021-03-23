#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"

class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, const char* anImage);
	~GameEntity(void);

	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Draw(Drawer* aDrawer);

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	const char* myImage;
};

#endif // GAMEENTITY_H