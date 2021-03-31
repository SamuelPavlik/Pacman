#pragma once

#include "GameEntity.h"

#include <memory>
#include <vector>

using GameCollection = std::vector<std::shared_ptr<GameEntity>>;
using GameIt = GameCollection::iterator;

class Collisions
{
public:
	static bool IsColliding(std::shared_ptr<GameEntity> entity1,
		std::shared_ptr<GameEntity> entity2)
	{
		return (entity1->GetPosition() - entity2->GetPosition()).Length() < COLLISION_DELTA;
	}

	static bool IsColliding(GameCollection& coll, 
		std::shared_ptr<GameEntity> entity, GameIt& it)
	{
		it = std::find_if(coll.begin(), coll.end(), [entity](auto dot) {
				return IsColliding(dot, entity);
			});

		return it != coll.end();
	}
};
