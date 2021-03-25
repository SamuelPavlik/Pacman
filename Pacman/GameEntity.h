#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"

#include <memory>

class Drawer;
class C_Sprite;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition);
	~GameEntity(void);

	void Start();
	void SetSprite(std::shared_ptr<Drawer> drawer, const char* name);
	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Update(float time);
	void Draw(std::shared_ptr<Drawer> drawer);
	std::shared_ptr<C_Sprite> GetSprite() const;

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	std::shared_ptr<C_Sprite> mySprite;
};

#endif // GAMEENTITY_H