#include "C_Sprite.h"
#include "Drawer.h"
#include "GameEntity.h"

C_Sprite::C_Sprite(GameEntity& owner, std::shared_ptr<Drawer> drawer, const char* name) :
	Component(owner),
	name(name)
{
	Load(drawer, name);
}

void C_Sprite::Draw(std::shared_ptr<Drawer> drawer)
{
	//TODO what are the magic nums?
	drawer->Draw(name, owner.GetPosition().myX + 220, owner.GetPosition().myY + 60);
}

void C_Sprite::SetName(const char* name)
{
	this->name = name;
}

void C_Sprite::Load(std::shared_ptr<Drawer> drawer, const char* name)
{
    if (drawer)
    {
		drawer->AddResource(name);
    }
}