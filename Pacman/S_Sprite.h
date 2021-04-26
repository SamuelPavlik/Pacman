#pragma once

#include <vector>
#include <memory>
#include <set>
#include <functional>

class GameEntity;
class Drawer;

using CompareFuncType = 
std::function<bool(const std::shared_ptr<GameEntity>&, const std::shared_ptr<GameEntity>&)>;

class S_Sprite
{
public:
	S_Sprite();

	void Add(const std::vector<std::shared_ptr<GameEntity>>& entities);
	void Add(const std::shared_ptr<GameEntity>& entity);

	void ProcessRemovals();
	void Clear();

	void Draw();

private:
	std::multiset<std::shared_ptr<GameEntity>, CompareFuncType> drawables;
};

