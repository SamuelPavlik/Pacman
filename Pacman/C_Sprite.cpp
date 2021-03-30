#include "C_Sprite.h"
#include "Drawer.h"
#include "GameEntity.h"
#include "Constants.h"

C_Sprite::C_Sprite(GameEntity& owner, Drawer* drawer, const char* name) :
	Component(owner),
	drawer(drawer),
	name(name)
{
	Load(name);
}

void C_Sprite::Draw()
{
	//TODO what are the magic nums?
	drawer->Draw(name, owner.GetPosition().myX + TOTAL_OFFSET_X, owner.GetPosition().myY + TOTAL_OFFSET_Y);
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