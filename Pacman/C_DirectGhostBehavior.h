#pragma once

#include "C_GhostBehavior.h"

class C_DirectGhostBehavior : public C_GhostBehavior {
public:
	C_DirectGhostBehavior(GameEntity& owner, const World& world,
		const std::shared_ptr<const GameEntity>& avatar,
		float moveSpeed = GHOST_SPEED);

protected:
	virtual Vector2f GetNextTile(const Vector2f& curr, bool isClaimableFlag) const override;
};

