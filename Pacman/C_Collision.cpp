#include "C_Collision.h"
#include "GameEntity.h"
#include "Constants.h"

C_Collision::C_Collision(GameEntity& owner, CollisionLayer layer) :
	Component(owner),
	layer(layer) {}

CollisionData C_Collision::IsColliding(const std::shared_ptr<C_Collision>& other)
{
	CollisionData cd;
	if ((owner.GetPosition() - other->owner.GetPosition()).Length() < COLLISION_DELTA)
	{
		cd.colliding = true;
		cd.other = &(other->owner);
	}
	return cd;
}

void C_Collision::Resolve(CollisionData cd)
{
	if (onOverlapFunc)
	{
		onOverlapFunc(cd);
	}
}

void C_Collision::BindOnOverlapFunc(std::function<void(CollisionData cd)> onOverlapFunc)
{
	this->onOverlapFunc = onOverlapFunc;
}
