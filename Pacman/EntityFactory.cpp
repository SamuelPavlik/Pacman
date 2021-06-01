#include "EntityFactory.h"
#include "Constants.h"
#include "Animation.h"
#include "C_Animation.h"
#include "C_KeyboardMovement.h"
#include "C_Sprite.h"
#include "C_PacmanProperties.h"
#include "C_AmbushGhostBehavior.h"
#include "C_DirectGhostBehavior.h"
#include "GameEntity.h"
#include "World.h"

EntityFactory::EntityFactory(Drawer& drawer, InputManager& inputManager, World& world) :
	drawer(drawer),
	inputManager(inputManager),
	world(world) {}

std::shared_ptr<GameEntity> EntityFactory::CreatePacman(const Vector2f& position, 
	std::function<void(CollisionData&)> onOverlapFunc,
	const char* spriteName)
{
	auto avatar = std::make_shared<GameEntity>(position);
	avatar->AddComponent<C_Sprite>(drawer, spriteName);
	avatar->AddComponent<C_KeyboardMovement>(inputManager, world);
	auto collision = avatar->AddComponent<C_Collision>(CollisionLayer::Player);
	collision->BindOnOverlapFunc(onOverlapFunc);

	//set up avatar animation
	auto avatarAnim = avatar->AddComponent<C_Animation>();
	auto goingLeftAnim = std::make_shared<Animation>();
	const float eatingFrameSeconds = 0.2f;
	goingLeftAnim->AddFrame(drawer, "closed_left_32.png", eatingFrameSeconds);
	goingLeftAnim->AddFrame(drawer, "open_left_32.png", eatingFrameSeconds);

	auto goingRightAnim = std::make_shared<Animation>();
	goingRightAnim->AddFrame(drawer, "closed_right_32.png", eatingFrameSeconds);
	goingRightAnim->AddFrame(drawer, "open_right_32.png", eatingFrameSeconds);

	auto goingUpAnim = std::make_shared<Animation>();
	goingUpAnim->AddFrame(drawer, "closed_up_32.png", eatingFrameSeconds);
	goingUpAnim->AddFrame(drawer, "open_up_32.png", eatingFrameSeconds);

	auto goingDownAnim = std::make_shared<Animation>();
	goingDownAnim->AddFrame(drawer, "closed_down_32.png", eatingFrameSeconds);
	goingDownAnim->AddFrame(drawer, "open_down_32.png", eatingFrameSeconds);

	avatarAnim->AddAnimation(AnimationState::GoingLeft, std::move(goingLeftAnim));
	avatarAnim->AddAnimation(AnimationState::GoingRight, std::move(goingRightAnim));
	avatarAnim->AddAnimation(AnimationState::GoingUp, std::move(goingUpAnim));
	avatarAnim->AddAnimation(AnimationState::GoingDown, std::move(goingDownAnim));

	avatar->AddComponent<C_PacmanProperties>();

	return avatar;
}

std::shared_ptr<GameEntity> EntityFactory::CreateDirectGhost(const Vector2f& position, 
	const Vector2f& spriteOffset, std::shared_ptr<GameEntity> avatar, const char* spriteName)
{
	auto ghost = std::make_shared<GameEntity>(position);
	ghost->AddComponent<C_Sprite>(drawer, spriteName, spriteOffset);
	ghost->AddComponent<C_DirectGhostBehavior>(world, avatar);

	return CreateGhost(ghost, spriteName);
}

std::shared_ptr<GameEntity> EntityFactory::CreateAmbushGhost(const Vector2f& position, 
	const Vector2f& spriteOffset, std::shared_ptr<GameEntity> avatar, const char* spriteName)
{
	auto ghost = std::make_shared<GameEntity>(position);
	ghost->AddComponent<C_Sprite>(drawer, spriteName, spriteOffset);
	ghost->AddComponent<C_AmbushGhostBehavior>(world, avatar);

	return CreateGhost(ghost, spriteName);
}

std::shared_ptr<GameEntity> EntityFactory::CreateDot(const Vector2f& position, const char* name)
{
	auto dot = std::make_shared<GameEntity>(position);
	dot->AddComponent<C_Sprite>(drawer, name);
	dot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	dot->tag = DOT_TAG;

	return dot;
}

std::shared_ptr<GameEntity> EntityFactory::CreateBigDot(const Vector2f& position, const char* name)
{
	auto bigDot = std::make_shared<GameEntity>(position);
	bigDot->AddComponent<C_Sprite>(drawer, name);
	bigDot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	bigDot->tag = BIG_DOT_TAG;

	return bigDot;
}

std::shared_ptr<GameEntity> EntityFactory::CreateGhost(std::shared_ptr<GameEntity>& ghost, const char* spriteName)
{
	ghost->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	ghost->tag = ENEMY_TAG;

	auto ghostAnim = ghost->AddComponent<C_Animation>();

	auto normalAnim = std::make_shared<Animation>();
	normalAnim->AddFrame(drawer, spriteName, 0.f);

	auto vulnerableAnim = std::make_shared<Animation>();
	vulnerableAnim->AddFrame(drawer, "Ghost_Vulnerable_32.png", 0.f);

	auto deadAnim = std::make_shared<Animation>();
	deadAnim->AddFrame(drawer, "Ghost_Dead_32.png", 0.f);

	ghostAnim->AddAnimation(AnimationState::GoingUp, std::move(normalAnim));
	ghostAnim->AddAnimation(AnimationState::Vulnerable, std::move(vulnerableAnim));
	ghostAnim->AddAnimation(AnimationState::Dead, std::move(deadAnim));

	return ghost;
}