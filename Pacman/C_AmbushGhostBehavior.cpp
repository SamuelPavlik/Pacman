#include "C_AmbushGhostBehavior.h"
#include "C_KeyboardMovement.h"
#include "GameEntity.h"
#include "World.h"

C_AmbushGhostBehavior::C_AmbushGhostBehavior(GameEntity& owner, const World& world, 
	const std::shared_ptr<const GameEntity>& avatar, float moveSpeed) : 
	C_GhostBehavior(owner, world, avatar, moveSpeed) {
	moveComp = avatar->GetComponent<C_KeyboardMovement>();
}

Vector2f C_AmbushGhostBehavior::GetNextTile(const Vector2f& position, bool isClaimableFlag) const {
	auto avatarPos = avatar->GetPosition() / TILE_SIZE;
	if (!moveComp || isClaimableFlag) {
		return avatarPos;
	}

	auto ahead = 0;
	auto unitDir = moveComp->GetDirection();
	auto len = (avatarPos - position).Length();
	auto nextPos = avatarPos;
	nextPos.x = std::floor(nextPos.x);
	nextPos.y = std::floor(nextPos.y);
	
	while (ahead < 4
		&& world.TileIsValid(nextPos.x + unitDir.x, nextPos.y + unitDir.y)
		&& nextPos != position) {
		ahead++;
		nextPos += unitDir;
	}
	
	if (nextPos == position) {
		return avatarPos;
	}

	return nextPos;
}
