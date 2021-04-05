#include "World.h"
#include "Vector2f.h"
#include "PathmapTile.h"
#include "GameEntity.h"
#include "Drawer.h"
#include "EntityFactory.h"
#include "Constants.h"

#include <sstream>
#include <fstream>
#include <string>

void World::Init(Drawer& drawer, EntityFactory& factory,
	std::vector<std::shared_ptr<GameEntity>>& dots,
	std::vector<std::shared_ptr<GameEntity>>& bigDots)
{
	//load map image
	drawer.AddResource("playfield.png");

	//init edible game entities and walls
	std::string line;
	std::ifstream myfile("map.txt");
	if (!myfile.is_open())
	{
		return;
	}

	int lineIndex = 0;
	while (std::getline(myfile, line))
	{
		tileMap.push_back(std::vector<std::shared_ptr<PathmapTile>>{});
		for (unsigned int i = 0; i < line.length(); i++)
		{
			tileMap[lineIndex].push_back(std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x')));

			if (line[i] == '.')
			{
				auto dot = factory.CreateDot(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE),
					"Small_Dot_32.png");
				dots.push_back(dot);
			}
			else if (line[i] == 'o')
			{
				auto bigDot = factory.CreateBigDot(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE),
					"Big_Dot_32.png");
				bigDots.push_back(bigDot);
			}
		}

		lineIndex++;
	}
	myfile.close();
}

void World::Draw(Drawer& drawer)
{
	drawer.Draw("playfield.png");
}

bool World::TileIsValid(int x, int y) const
{
	if (x < 0
		|| x >= tileMap[0].size()
		|| y < 0
		|| y >= tileMap.size())
	{
		return false;
	}

	return !tileMap[y][x]->isBlockingFlag;
}

const std::vector<std::vector<std::shared_ptr<PathmapTile>>> World::GetMap() const
{
	return tileMap;
}