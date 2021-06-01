#pragma once

#include "C_GhostBehavior.h"

class C_AmbushGhostBehavior : public C_GhostBehavior {
public:
	C_AmbushGhostBehavior(GameEntity& owner, const World& world,
		const std::shared_ptr<const GameEntity>& avatar,
		float moveSpeed = GHOST_SPEED) noexcept;

protected:
	virtual Vector2f GetNextTile(const Vector2f& curr, bool isClaimableFlag) const override;

private:
	std::shared_ptr<const class C_KeyboardMovement> moveComp;
};

