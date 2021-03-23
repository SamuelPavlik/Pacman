#ifndef CHERRY_H
#define CHERRY_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Cherry : public StaticGameEntity
{
public:
	Cherry(Vector2f aPosition);
	~Cherry(void);
};

#endif // CHERRY_H