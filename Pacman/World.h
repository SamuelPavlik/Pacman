#pragma once

#include <memory>
#include <vector>

class Drawer;
class PathmapTile;
class GameEntity;
class EntityManager;
class EntityFactory;

class World
{
public:
	void Init(Drawer& drawer, EntityFactory& factory,
		std::vector<std::shared_ptr<GameEntity>>& dots,
		std::vector<std::shared_ptr<GameEntity>>& bigDots);

	void Draw(Drawer& drawer);
	bool TileIsValid(int anX, int anY) const;
	const std::vector<std::vector<std::shared_ptr<PathmapTile>>> GetMap() const;
	
private:
	std::vector<std::vector<std::shared_ptr<PathmapTile>>> myPathmapTiles;
};