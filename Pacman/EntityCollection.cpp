#include "EntityCollection.h"
#include "GameEntity.h"
#include "SDL.h"
#include "SDL_image.h"

#include <algorithm>

EntityCollection::EntityCollection(std::shared_ptr<S_Sprite> spriteSys) :
    spriteSys(spriteSys) {}

void EntityCollection::Update(float deltaTime)
{
    for (auto o : entities)
    {
        o->Update(deltaTime);
    }
}

void EntityCollection::LateUpdate(float deltaTime)
{
    for (auto& o : entities)
    {
        o->LateUpdate(deltaTime);
    }
}

void EntityCollection::Draw(std::shared_ptr<Drawer> aDrawer)
{
    //drawables.Draw(aDrawer);
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
            o->Start();
        }

        entities.insert(entities.end(), newEntities.begin(), newEntities.end());
        ProcessNewResources(entities);
        newEntities.clear();
    }
}

void EntityCollection::ProcessNewResources(std::vector<std::shared_ptr<GameEntity>> entities)
{
    for (auto entity : newEntities)
    {
        auto anImage = entity->GetImage();
        if (resourceMap.find(anImage) == resourceMap.end())
        {
            SDL_Surface* surface = IMG_Load(anImage);
            if (!surface)
                return;

            resourceMap[anImage] = std::shared_ptr<SDL_Texture>(
                SDL_CreateTextureFromSurface(aRenderer, surface));
        }
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
        drawables.ProcessRemovals();
        collidables.ProcessRemovals();
    }
}