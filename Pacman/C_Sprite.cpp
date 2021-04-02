#include "C_Sprite.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"

C_Sprite::C_Sprite(GameEntity& owner, Drawer* drawer, const char* name, 
	Vector2f offset, bool isVisible) :
	Component(owner),
	drawer(drawer),
	name(name),
	offset(offset),
	isVisible(isVisible)
{
	Load(name);
}

void C_Sprite::Draw()
{
	if (isVisible)
	{
		drawer->Draw(name, owner.GetPosition().myX + TOTAL_OFFSET_X + offset.myX, 
			owner.GetPosition().myY + TOTAL_OFFSET_Y + offset.myY);
	}
}

void C_Sprite::SetName(const char* name)
{
	this->name = name;
}

void C_Sprite::Load(const char* name)
{
    if (drawer)
    {
		drawer->AddResource(name);
    }
}