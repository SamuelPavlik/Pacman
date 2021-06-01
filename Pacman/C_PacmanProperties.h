#pragma once

#include "Component.h"

#include <memory>

class C_Sprite;
class C_Collision;

class C_PacmanProperties : public Component
{
public:
	C_PacmanProperties(GameEntity& owner) noexcept;

	void Awake() override;
	void Start() override;
	void Update(float time) override;

private:
	float invulnerableTimer;
	float visibilityTimer;
	std::shared_ptr<C_Sprite> sprite;
	std::shared_ptr<C_Collision> collision;
};

