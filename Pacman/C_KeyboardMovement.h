#pragma once

#include "Component.h"
#include "Vector2f.h"
#include "Constants.h"

#include <memory>

class GameEntity;
class InputManager;
class World;
class C_Animation;

class C_KeyboardMovement : public Component
{
public:
	C_KeyboardMovement(GameEntity& owner, InputManager& inputManager, const World& world, 
		float moveSpeed = PACMAN_SPEED) noexcept;

	void SetMoveSpeed(float moveSpeed) noexcept;
	const Vector2f GetDirection() const noexcept;

	void Awake() override;
	void Start() override;
	void Update(float time) override;

private:
	void Move(float time, Vector2f possibleMove);

	float moveSpeed;
	InputManager& inputManager;
	const World& world;
	Vector2f nextMovement;
	std::shared_ptr<C_Animation> animation;

	int currentTileX;
	int currentTileY;
	int nextTileX;
	int nextTileY;
};

