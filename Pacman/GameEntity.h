#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"

#include <memory>

class Drawer;
class C_Sprite;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, const char* anImage);
	~GameEntity(void);

	void Start();
	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Draw(std::shared_ptr<Drawer> aDrawer);
	const char* GetImage();
	std::shared_ptr<C_Sprite> GetSprite() const;

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	const char* myImage;
	std::shared_ptr<C_Sprite> mySprite;
};

#endif // GAMEENTITY_H