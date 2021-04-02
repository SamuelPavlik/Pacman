#pragma once

class PathmapTile
{
public:
	PathmapTile(int x, int y, bool isBlockingFlag);

	int x;
	int y;
	bool isBlockingFlag;
};