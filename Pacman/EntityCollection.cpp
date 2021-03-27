#include "EntityCollection.h"
#include "GameEntity.h"

#include <algorithm>

void EntityCollection::Update(float deltaTime)
{
    for (auto o : entities)
    {
        o->Update(deltaTime);
    }
}

void EntityCollection::Draw()
{
    spriteSys.Draw();
}

void EntityCollection::Add(std::shared_ptr<GameEntity> entity)
{
    newEntities.push_back(entity);
}

void EntityCollection::Add(std::vector<std::shared_ptr<GameEntity>> entities)
{
    newEntities.insert(newEntities.end(), entities.begin(), entities.end());
}

void EntityCollection::ProcessNewEntities()
{
    if (newEntities.size() > 0)
    {
        for (const auto& o : newEntities)
        {
            o->Awake();
        }
        for (const auto& o : newEntities)
        {
            o->Start();
        }

        entities.insert(entities.end(), newEntities.begin(), newEntities.end());
        spriteSys.Add(entities);
        newEntities.clear();
    }
}

void EntityCollection::ProcessRemovals()
{
    bool isRemovedSth = false;
    auto newEnd = std::remove_if(entities.begin(), entities.end(),
        [&isRemovedSth](std::shared_ptr<GameEntity> obj) {
            if (obj->IsMarkedForDelete())
            {
                isRemovedSth = true;
                return true;
            }
            return false;
        });
    entities.erase(newEnd, entities.end());

    if (isRemovedSth)
    {
        spriteSys.ProcessRemovals();
    }
}