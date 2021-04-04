#pragma once

class PathmapTile
{
public:
	PathmapTile::PathmapTile(int x, int y, bool isBlockingFlag) :
		x(x),
		y(y),
		isBlockingFlag(isBlockingFlag) {}

	int x;
	int y;
	bool isBlockingFlag;
};