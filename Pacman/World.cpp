#include "World.h"
#include "PathmapTile.h"
#include "GameEntity.h"
#include "Drawer.h"
#include "Constants.h"
#include "C_Sprite.h"
#include "C_Collision.h"
#include "EntityCollection.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

World::World(void) {}

World::~World(void) {}

void World::Init(Drawer* drawer,
	std::vector<std::shared_ptr<GameEntity>>& dots,
	std::vector<std::shared_ptr<GameEntity>>& bigDots)
{
	//load map image
	drawer->AddResource("playfield.png");

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
					auto dot = std::make_shared<GameEntity>(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE));
					dot->AddComponent<C_Sprite>(drawer, "Small_Dot_32.png");
					dot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
					dot->tag = DOT_TAG;
					dots.push_back(dot);
				}
				else if (line[i] == 'o')
				{
					auto bigDot = std::make_shared<GameEntity>(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE));
					bigDot->AddComponent<C_Sprite>(drawer, "Big_Dot_32.png");
					bigDot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
					bigDot->tag = BIG_DOT_TAG;
					bigDots.push_back(bigDot);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}
}

void World::Draw(Drawer* drawer)
{
	drawer->Draw("playfield.png");
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