#include "C_DirectGhostBehavior.h"
#include "GameEntity.h"

C_DirectGhostBehavior::C_DirectGhostBehavior(GameEntity& owner, const World& world, 
	const std::shared_ptr<const GameEntity>& avatar, float moveSpeed) noexcept :
	C_GhostBehavior(owner, world, avatar, moveSpeed) {}

Vector2f C_DirectGhostBehavior::GetNextTile(const Vector2f& curr, bool isClaimableFlag) const {
	return avatar->GetPosition() / TILE_SIZE;
}
