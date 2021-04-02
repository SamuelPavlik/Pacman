#include "EntityFactory.h"
#include "Constants.h"
#include "C_Animation.h"
#include "C_KeyboardMovement.h"
#include "C_Sprite.h"
#include "C_GhostBehavior.h"
#include "C_PacmanProperties.h"
#include "GameEntity.h"

EntityFactory::EntityFactory(Drawer& drawer, InputManager& inputManager, World& world) :
	drawer(drawer),
	inputManager(inputManager),
	world(world) {}

std::shared_ptr<GameEntity> EntityFactory::CreatePacman(Vector2f position, 
	std::function<void(CollisionData)> onOverlapFunc,
	const char* spriteName)
{
	auto myAvatar = std::make_shared<GameEntity>(position);
	myAvatar->AddComponent<C_Sprite>(&drawer, spriteName);
	myAvatar->AddComponent<C_KeyboardMovement>(&inputManager, &world);
	auto collision = myAvatar->AddComponent<C_Collision>(CollisionLayer::Player);
	collision->BindOnOverlapFunc(onOverlapFunc);

	//set up avatar animation
	auto avatarAnim = myAvatar->AddComponent<C_Animation>();
	auto goingLeftAnim = std::make_shared<Animation>();
	const float eatingFrameSeconds = 0.2f;
	goingLeftAnim->AddFrame(&drawer, "closed_left_32.png", eatingFrameSeconds);
	goingLeftAnim->AddFrame(&drawer, "open_left_32.png", eatingFrameSeconds);

	auto goingRightAnim = std::make_shared<Animation>();
	goingRightAnim->AddFrame(&drawer, "closed_right_32.png", eatingFrameSeconds);
	goingRightAnim->AddFrame(&drawer, "open_right_32.png", eatingFrameSeconds);

	auto goingUpAnim = std::make_shared<Animation>();
	goingUpAnim->AddFrame(&drawer, "closed_up_32.png", eatingFrameSeconds);
	goingUpAnim->AddFrame(&drawer, "open_up_32.png", eatingFrameSeconds);

	auto goingDownAnim = std::make_shared<Animation>();
	goingDownAnim->AddFrame(&drawer, "closed_down_32.png", eatingFrameSeconds);
	goingDownAnim->AddFrame(&drawer, "open_down_32.png", eatingFrameSeconds);

	avatarAnim->AddAnimation(AnimationState::GoingLeft, goingLeftAnim);
	avatarAnim->AddAnimation(AnimationState::GoingRight, goingRightAnim);
	avatarAnim->AddAnimation(AnimationState::GoingUp, goingUpAnim);
	avatarAnim->AddAnimation(AnimationState::GoingDown, goingDownAnim);

	myAvatar->AddComponent<C_PacmanProperties>();

	return myAvatar;
}

std::shared_ptr<GameEntity> EntityFactory::CreateGhost(Vector2f position, Vector2f spriteOffset, 
	std::shared_ptr<GameEntity> avatar, const char* spriteName)
{
	auto myGhost = std::make_shared<GameEntity>(Vector2f(GHOST_START_TILE_X * TILE_SIZE,
		GHOST_START_TILE_Y * TILE_SIZE));
	myGhost->AddComponent<C_Sprite>(&drawer, "ghost_32.png", spriteOffset);
	myGhost->AddComponent<C_GhostBehavior>(&world, avatar);
	myGhost->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	myGhost->tag = ENEMY_TAG;

	auto ghostAnim = myGhost->AddComponent<C_Animation>();

	auto normalAnim = std::make_shared<Animation>();
	normalAnim->AddFrame(&drawer, "ghost_32.png", 0.f);

	auto vulnerableAnim = std::make_shared<Animation>();
	vulnerableAnim->AddFrame(&drawer, "Ghost_Vulnerable_32.png", 0.f);

	auto deadAnim = std::make_shared<Animation>();
	deadAnim->AddFrame(&drawer, "Ghost_Dead_32.png", 0.f);

	ghostAnim->AddAnimation(AnimationState::GoingUp, normalAnim);
	ghostAnim->AddAnimation(AnimationState::Vulnerable, vulnerableAnim);
	ghostAnim->AddAnimation(AnimationState::Dead, deadAnim);

	return myGhost;
}

std::shared_ptr<GameEntity> EntityFactory::CreateDot(Vector2f position, const char* name)
{
	auto dot = std::make_shared<GameEntity>(position);
	dot->AddComponent<C_Sprite>(&drawer, name);
	dot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	dot->tag = DOT_TAG;

	return dot;
}

std::shared_ptr<GameEntity> EntityFactory::CreateBigDot(Vector2f position, const char* name)
{
	auto bigDot = std::make_shared<GameEntity>(position);
	bigDot->AddComponent<C_Sprite>(&drawer, name);
	bigDot->AddComponent<C_Collision>(CollisionLayer::NonPlayer);
	bigDot->tag = BIG_DOT_TAG;

	return bigDot;
}
