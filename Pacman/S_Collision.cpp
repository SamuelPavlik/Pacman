#include "S_Collision.h"
#include "C_Collision.h"
#include "GameEntity.h"

void S_Collision::Add(std::vector<std::shared_ptr<GameEntity>>& entities)
{
    for (auto entity : entities)
    {
        auto collider = entity->GetComponent<C_Collision>();
        if (collider)
        {
            collidables[collider->layer].push_back(collider);
        }
    }
}

void S_Collision::ProcessRemovals()
{
    for (auto& layer : collidables)
    {
        auto newEnd = std::remove_if(layer.second.begin(), layer.second.end(), [](auto coll) {
            return coll->owner.IsMarkedForDelete();
            });
        layer.second.erase(newEnd, layer.second.end());
    }
}

void S_Collision::Update()
{
    for (auto maps : collidables)
    {
        if (maps.first == CollisionLayer::NonPlayer)
        {
            continue;
        }

        for (auto collidableObj : maps.second)
        {
            if (!collidableObj->isComponentOn)
                continue;

            std::vector<std::shared_ptr<C_Collision>> collisions = collidables[CollisionLayer::NonPlayer];

            for (auto collisionObj : collisions)
            {
                CollisionData cd = collidableObj->IsColliding(collisionObj);

                if (cd.colliding)
                {
                    collidableObj->Resolve(cd);
                }
            }
        }
    }
}

void S_Collision::Clear()
{
    collidables.clear();
}
