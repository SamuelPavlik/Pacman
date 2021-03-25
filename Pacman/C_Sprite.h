#pragma once

#include "Component.h"
#include "SDL.h"

#include <memory>

class Drawer;
class GameEntity;

class C_Sprite : public Component
{
public:
	C_Sprite(GameEntity& owner, std::shared_ptr<Drawer> drawer, const char* name);
	
	void Draw(std::shared_ptr<Drawer> drawer);
	void Load(std::shared_ptr<Drawer> drawer, const char* name);
	void SetName(const char* name);

	int sortOrder;

private:
	const char* name;
};

