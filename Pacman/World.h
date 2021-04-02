#ifndef WORLD_H
#define WORLD_H

#include "Vector2f.h"

#include <memory>
#include <list>
#include <vector>

class Drawer;
class PathmapTile;
class GameEntity;
class EntityManager;
class EntityFactory;

class World
{
public:
	World(void);
	~World(void);

	void Init(Drawer& drawer, EntityFactory& factory,
		std::vector<std::shared_ptr<GameEntity>>& dots,
		std::vector<std::shared_ptr<GameEntity>>& bigDots);

	void Draw(Drawer* drawer);
	bool TileIsValid(int anX, int anY) const;
	const std::vector<std::vector<std::shared_ptr<PathmapTile>>> GetMap() const;
	
private:

	std::vector<std::vector<std::shared_ptr<PathmapTile>>> myPathmapTiles;
};

#endif // WORLD_H