#pragma once

#include "Component.h"
#include "Vector2f.h"

#include <memory>

class Drawer;
class GameEntity;

class C_Sprite : public Component
{
public:
	C_Sprite(GameEntity& owner, Drawer& drawer, const char* name, 
		Vector2f offset = Vector2f());
	
	void Draw();
	void Load(const char* name);
	void SetName(const char* name);

	int sortOrder;

private:
	const char* name;
	Drawer& drawer;
	Vector2f offset;
};

