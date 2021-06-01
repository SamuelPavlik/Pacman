#include "S_Sprite.h"
#include "GameEntity.h"
#include "C_Sprite.h"
#include "Drawer.h"

#include <algorithm>

S_Sprite::S_Sprite()
{
	auto cmp = [](const auto& a, const auto& b) {
		return a->sortOrder < b->sortOrder; 
		};
	drawables = std::multiset<std::shared_ptr<C_Sprite>, CompareFuncType>(cmp);
}

void S_Sprite::Add(const std::vector<std::shared_ptr<GameEntity>>& objects)
{
	for (auto& o : objects)
	{
		Add(o);
	}
}

void S_Sprite::Add(const std::shared_ptr<GameEntity>& entity)
{
	if (auto elem = entity->GetSprite())
	{
		drawables.insert(std::move(elem));
	}
}

void S_Sprite::ProcessRemovals()
{
	for (auto i = drawables.begin(), last = drawables.end(); i != last; )
	{
		if ((*i)->owner.IsMarkedForDelete())
		{
			i = drawables.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void S_Sprite::Clear() noexcept
{
	drawables.clear();
}

void S_Sprite::Draw()
{
	for (auto& d : drawables)
	{
		if (d && d->isComponentOn) 
		{
			d->Draw();
		}
	}
}