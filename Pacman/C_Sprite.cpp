#include "C_Sprite.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"

C_Sprite::C_Sprite(GameEntity& owner, Drawer& drawer, const char* name, 
	Vector2f offset) :
	Component(owner),
	drawer(drawer),
	name(name),
	offset(offset)
{
	Load(name);
}

void C_Sprite::Draw()
{
	drawer.Draw(name, owner.GetPosition().x + TOTAL_OFFSET_X + offset.x, 
		owner.GetPosition().y + TOTAL_OFFSET_Y + offset.y);
}

void C_Sprite::SetName(const char* name) noexcept
{
	this->name = name;
}

void C_Sprite::Load(const char* name)
{
	drawer.AddResource(name);
}