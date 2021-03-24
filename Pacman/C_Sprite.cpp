#include "C_Sprite.h"
#include "Drawer.h"

C_Sprite::C_Sprite(GameEntity& owner, std::shared_ptr<Drawer> drawer, const char* name) :
	owner(owner),
	drawer(drawer)
{
	Load(name);
}

void C_Sprite::Load(const char* name)
{
    if (drawer)
    {
		drawer->AddResource(name);
    }
}

void C_Sprite::Draw()
{
}
