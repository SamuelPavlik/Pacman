#include "EntityManager.h"
#include "GameEntity.h"

#include <algorithm>

void EntityManager::Update(float deltaTime)
{
    for (auto o : entities)
    {
        o->Update(deltaTime);
    }
    collisionSys.Update();
}

void EntityManager::Draw()
{
    spriteSys.Draw();
}

void EntityManager::Add(std::shared_ptr<GameEntity> entity)
{
    newEntities.push_back(entity);
}

void EntityManager::Add(std::vector<std::shared_ptr<GameEntity>> entities)
{
    newEntities.insert(newEntities.end(), entities.begin(), entities.end());
}

void EntityManager::ProcessNewEntities()
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
        collisionSys.Add(entities);
        newEntities.clear();
    }
}

void EntityManager::ProcessRemovals()
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
        collisionSys.ProcessRemovals();
    }
}

void EntityManager::Clear()
{
    entities.clear();
    newEntities.clear();
    collisionSys.Clear();
    spriteSys.Clear();
}
