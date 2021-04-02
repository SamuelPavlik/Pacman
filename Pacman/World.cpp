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
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (!myfile.eof())
		{
			myPathmapTiles.push_back(std::vector<std::shared_ptr<PathmapTile>>{});
			std::getline(myfile, line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				myPathmapTiles[lineIndex].push_back(std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x')));

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
}

void World::Draw(Drawer& drawer)
{
	drawer.Draw("playfield.png");
}

bool World::TileIsValid(int anX, int anY) const
{
	if (anX < 0) return false;
	if (anX >= myPathmapTiles[0].size()) return false;
	if (anY < 0) return false;
	if (anY >= myPathmapTiles.size()) return false;
	return !myPathmapTiles[anY][anX]->myIsBlockingFlag;
}

const std::vector<std::vector<std::shared_ptr<PathmapTile>>> World::GetMap() const
{
	return myPathmapTiles;
}