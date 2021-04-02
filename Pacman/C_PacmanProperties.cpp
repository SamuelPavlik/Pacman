#include "C_PacmanProperties.h"
#include "GameEntity.h"
#include "C_Sprite.h"
#include "C_Collision.h"
#include "Constants.h"

C_PacmanProperties::C_PacmanProperties(GameEntity& owner) :
	Component(owner),
	invulnerableTimer(),
	visibilityTimer()
{}

void C_PacmanProperties::Awake()
{
	sprite = owner.GetComponent<C_Sprite>();
	collision = owner.GetComponent<C_Collision>();
}

void C_PacmanProperties::Start()
{
	invulnerableTimer = PACMAN_INVULNERABLE_COUNTER;
	visibilityTimer = PACMAN_VISIBILITY_COUNTER;
	collision->isCollisionOn = false;
}

void C_PacmanProperties::Update(float time)
{
	if (invulnerableTimer > 0.f)
	{
		if (visibilityTimer < 0.f)
		{
			sprite->isVisible = !sprite->isVisible;
			visibilityTimer = PACMAN_VISIBILITY_COUNTER;
		}
		invulnerableTimer -= time;
		visibilityTimer -= time;
	}
	else
	{
		sprite->isVisible = true;
		collision->isCollisionOn = true;
	}
}
