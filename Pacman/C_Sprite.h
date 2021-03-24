#pragma once

#include "SDL.h"

#include <unordered_map>
#include <memory>

class Drawer;
class GameEntity;

class C_Sprite
{
public:
	C_Sprite(GameEntity& owner, std::shared_ptr<Drawer> drawer, const char* name);
	
	void Update(float deltaTime);
	void Draw();
	void Load(const char* name);

	void SetSortOrder(int order);
	int GetSortOrder() const;

protected:
	GameEntity& owner;

private:
	int sortOrder;
	std::shared_ptr<Drawer> drawer;
	std::shared_ptr<SDL_Texture> texture;
};

