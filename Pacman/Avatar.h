#ifndef AVATAR_H
#define AVATAR_H

#include "GameEntity.h"
#include "Vector2f.h"

class C_KeyboardMovement;
class Input;
class World;

class Avatar : public GameEntity
{
public:
	Avatar(const Vector2f& aPosition);
	~Avatar(void);

	//void SetMovement(Input& input, const World& world);
	//void Update(float aTime) override;

private:
	std::shared_ptr<C_KeyboardMovement> movement;

};

#endif //AVATAR_H