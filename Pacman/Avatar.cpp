#include "Avatar.h"
#include "Constants.h"
#include "C_KeyboardMovement.h"

Avatar::Avatar(const Vector2f& aPosition)
: GameEntity(aPosition) {}

Avatar::~Avatar(void) {}

//void Avatar::SetMovement(Input& input, const World& world)
//{
//	movement = std::make_shared<C_KeyboardMovement>(*this, input, world);
//}
//
//void Avatar::Update(float aTime)
//{
//	movement->Update(aTime);
//}
