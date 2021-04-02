#pragma once

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);

	int myX;
	int myY;
	bool myIsBlockingFlag;
};