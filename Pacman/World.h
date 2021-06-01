#pragma once

#include <memory>
#include <vector>

class Drawer;
class PathmapTile;
class GameEntity;
class EntityManager;
class EntityFactory;

using PathmapTiles = std::vector<std::vector<std::shared_ptr<const PathmapTile>>>;

class World
{
public:
	void Init(Drawer& drawer, EntityFactory& factory,
		std::vector<std::shared_ptr<GameEntity>>& dots,
		std::vector<std::shared_ptr<GameEntity>>& bigDots);

	void Draw(Drawer& drawer);
	bool TileIsValid(int x, int y) const;
	const PathmapTiles GetMap() const noexcept;
	
private:
	PathmapTiles tileMap;
};