#ifndef WORLD_H
#define WORLD_H

#include "Vector2f.h"
#include <memory>
#include <list>
#include <vector>

class Drawer;
class PathmapTile;
class GameEntity;

class World
{
public:
	World(void);
	~World(void);

	void Init(Drawer* drawer,
		std::vector<std::shared_ptr<GameEntity>>& myDots,
		std::vector<std::shared_ptr<GameEntity>>& myBigDots);

	void Draw(Drawer* drawer);
	bool TileIsValid(int anX, int anY) const;
	const std::vector<std::vector<std::shared_ptr<PathmapTile>>> GetMap() const;
	
private:
	bool InitPathmap(Drawer* drawer,
		std::vector<std::shared_ptr<GameEntity>>& myDots,
		std::vector<std::shared_ptr<GameEntity>>& myBigDots);

	std::vector<std::vector<std::shared_ptr<PathmapTile>>> myPathmapTiles;
};

#endif // WORLD_H