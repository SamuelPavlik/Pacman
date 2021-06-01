#pragma once

#include <vector>
#include <memory>
#include <set>
#include <functional>

class GameEntity;
class Drawer;
class C_Sprite;

using CompareFuncType = 
std::function<bool(const std::shared_ptr<C_Sprite>&, const std::shared_ptr<C_Sprite>&)>;

class S_Sprite
{
public:
	S_Sprite();

	void Add(const std::vector<std::shared_ptr<GameEntity>>& entities);
	void Add(const std::shared_ptr<GameEntity>& entity);

	void ProcessRemovals();
	void Clear() noexcept;

	void Draw();

private:
	std::multiset<std::shared_ptr<C_Sprite>, CompareFuncType> drawables;
};

