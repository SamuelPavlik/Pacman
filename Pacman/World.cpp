#include "World.h"
#include "PathmapTile.h"
#include "GameEntity.h"
#include "Drawer.h"
#include "Constants.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

World::World(void)
{
}

World::~World(void)
{
}

void World::Init(std::shared_ptr<Drawer> drawer)
{
	InitPathmap(drawer);
}

bool World::InitPathmap(std::shared_ptr<Drawer> drawer)
{
	//load map image
	drawer->AddResource("playfield.png");

	//init edible game entities and walls
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			myPathmapTiles.push_back(std::vector<std::shared_ptr<PathmapTile>>{});
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				myPathmapTiles[lineIndex].push_back(std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x')));

				if (line[i] == '.')
				{				
					auto dot = std::make_shared<GameEntity>(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE));
					dot->SetSprite(drawer, "Small_Dot_32.png");
					myDots.push_back(dot);
				}
				else if (line[i] == 'o')
				{
					auto bigDot = std::make_shared<GameEntity>(Vector2f(i * TILE_SIZE, lineIndex * TILE_SIZE));
					bigDot->SetSprite(drawer, "Big_Dot_32.png");
					myBigDots.push_back(bigDot);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

void World::Draw(std::shared_ptr<Drawer> drawer)
{
	drawer->Draw("playfield.png");

	for(auto dot : myDots)
	{
		dot->Draw(drawer);
	}

	for(auto bigDot : myBigDots)
	{
		bigDot->Draw(drawer);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	if (anX < 0) return false;
	if (anX >= myPathmapTiles[0].size()) return false;
	if (anY < 0) return false;
	if (anY >= myPathmapTiles.size()) return false;
	return !myPathmapTiles[anY][anX]->myIsBlockingFlag;
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	auto dotIt = std::find_if(myDots.begin(), myDots.end(), [&aPosition](auto dot) {
			return (dot->GetPosition() - aPosition).Length() < 5.f;
		});
	if (dotIt != myDots.end())
	{
		myDots.erase(dotIt);
		return true;
	}
	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	auto bigDotIt = std::find_if(myBigDots.begin(), myBigDots.end(), [&aPosition](auto dot) {
		return (dot->GetPosition() - aPosition).Length() < 5.f;
		});
	if (bigDotIt != myBigDots.end())
	{
		myBigDots.erase(bigDotIt);
		return true;
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<std::shared_ptr<PathmapTile>>& aList)
{
	auto fromTile = myPathmapTiles[aFromY][aFromX];
	auto toTile = myPathmapTiles[aToY][aToX];

	for(auto line : myPathmapTiles)
	{
		for (auto tile : line)
		{
			tile->myIsVisitedFlag = false;
		}
	}

	Pathfind(fromTile, toTile, aList);
}

bool World::ListDoesNotContain(std::shared_ptr<PathmapTile> aFromTile, std::list<std::shared_ptr<PathmapTile>>& aList)
{
	return aList.cend() == std::find(aList.cbegin(), aList.cend(), aFromTile);
}

bool SortFromGhostSpawn(std::shared_ptr<PathmapTile> a, std::shared_ptr<PathmapTile> b)
{
	int la = abs(a->myX - 13) + abs(a->myY - 13);
	int lb = abs(b->myX - 13) + abs(b->myY - 13);

    return la < lb;
}

bool World::Pathfind(std::shared_ptr<PathmapTile> aFromTile, std::shared_ptr<PathmapTile> aToTile, std::list<std::shared_ptr<PathmapTile>>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<std::shared_ptr<PathmapTile>> neighborList;

	std::shared_ptr<PathmapTile> up = myPathmapTiles[aFromTile->myY - 1][aFromTile->myX];
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.push_front(up);

	std::shared_ptr<PathmapTile> down = myPathmapTiles[aFromTile->myY + 1][aFromTile->myX];
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.push_front(down);

	std::shared_ptr<PathmapTile> right = myPathmapTiles[aFromTile->myY][aFromTile->myX + 1];
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.push_front(right);

	std::shared_ptr<PathmapTile> left = myPathmapTiles[aFromTile->myY][aFromTile->myX - 1];
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for(auto tile : neighborList)
	{
		aList.push_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}