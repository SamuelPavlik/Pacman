#ifndef WORLD_H
#define WORLD_H

#include "Vector2f.h"
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;

class World
{
public:
	World(void);
	~World(void);

	void Init();

	void Draw(std::shared_ptr<Drawer> aDrawer);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	void Update();

	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<std::shared_ptr<PathmapTile>>& aList);

private:

	bool Pathfind(std::shared_ptr<PathmapTile> aFromTile, std::shared_ptr<PathmapTile> aToTile, std::list<std::shared_ptr<PathmapTile>>& aList);
	bool ListDoesNotContain(std::shared_ptr<PathmapTile> aFromTile, std::list<std::shared_ptr<PathmapTile>>& aList);


	bool InitPathmap();
	//bool InitDots();
	//bool InitBigDots();

	std::vector<std::vector<std::shared_ptr<PathmapTile>>> myPathmapTiles;
	std::vector<std::shared_ptr<Dot>> myDots;
	std::vector<std::shared_ptr<BigDot>> myBigDots;
	std::list<Cherry*> myCherry;

};

#endif // WORLD_H