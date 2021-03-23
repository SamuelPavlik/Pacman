#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);
	~PathmapTile(void);

	int myX;
	int myY;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;

};

#endif // PATHMAPTILE_H