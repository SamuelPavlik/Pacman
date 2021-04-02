#include "S_Sprite.h"
#include "GameEntity.h"
#include "C_Sprite.h"
#include "Drawer.h"

S_Sprite::S_Sprite()
{
    auto cmp = [](auto a, auto b) {
        return a->GetSprite()->sortOrder
            < b->GetSprite()->sortOrder; 
        };
    drawables =
        std::multiset<std::shared_ptr<GameEntity>,
        std::function<bool(std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>)>>(cmp);
}

void S_Sprite::Add(std::vector<std::shared_ptr<GameEntity>> objects)
{
    for (auto o : objects)
    {
        Add(o);
    }
}

void S_Sprite::Add(std::shared_ptr<GameEntity> entity)
{
    if (entity->GetSprite())
    {
        drawables.insert(entity);
    }
}

void S_Sprite::ProcessRemovals()
{
    for (auto i = drawables.begin(), last = drawables.end(); i != last; )
    {
        if ((*i)->IsMarkedForDelete())
            i = drawables.erase(i);
        else
            ++i;
    }
}

void S_Sprite::Clear()
{
    drawables.clear();
}

void S_Sprite::Draw()
{
    for (auto d : drawables)
        d->Draw();
}