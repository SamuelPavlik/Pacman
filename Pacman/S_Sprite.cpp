#include "S_Sprite.h"
#include "GameEntity.h"
#include "C_Sprite.h"

S_Sprite::S_Sprite()
{
    auto cmp = [](auto a, auto b) {
        return a->GetSprite()->GetSortOrder()
            < b->GetDrawable()->GetSortOrder(); };
    drawables =
        std::multiset<std::shared_ptr<GameEntity>,
        std::function<bool(std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>)>>(cmp);
}

void S_Sprite::Add(std::vector<std::shared_ptr<GameEntity>>& objects)
{
    for (auto o : objects) Add(o);
}

void S_Sprite::ProcessRemovals()
{
    for (auto i = drawables.begin(), last = drawables.end(); i != last; )
    {
        if ((*i)->IsQueuedForRemoval())
            i = drawables.erase(i);
        else
            ++i;
    }
}

void S_Sprite::Draw(std::shared_ptr<Drawer> aDrawer)
{
    for (auto d : drawables)
        d->Draw(window);
}

void S_Sprite::Add(std::shared_ptr<GameEntity> object)
{
    if (object->GetDrawable())
    {
        drawables.insert(object);
    }
}