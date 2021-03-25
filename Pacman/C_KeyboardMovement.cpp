#include "C_KeyboardMovement.h"
#include "Input.h"
#include "World.h"
#include "GameEntity.h"
#include "Constants.h"

C_KeyboardMovement::C_KeyboardMovement(GameEntity& owner, 
    Input& input, const World& world) : 
    Component(owner), 
    moveSpeed(200.f), 
    input(input),
    world(world),
    nextMovement(-1.f, 0.f) {}

void C_KeyboardMovement::SetMoveSpeed(float moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

void C_KeyboardMovement::Update(float time)
{
    if (input.IsKeyDown(Input::Key::Up))
        nextMovement = Vector2f(0.f, -1.f);
    else if (input.IsKeyDown(Input::Key::Down))
        nextMovement = Vector2f(0.f, 1.f);
    else if (input.IsKeyDown(Input::Key::Left))
        nextMovement = Vector2f(-1.f, 0.f);
    else if (input.IsKeyDown(Input::Key::Right))
        nextMovement = Vector2f(1.f, 0.f);

    Move(nextMovement, time);
}

void C_KeyboardMovement::Move(Vector2f nextMove, float time)
{
    //auto nextPos = owner.GetPosition() + nextMove;
    int nextTileX = owner.GetCurrentTileX() + nextMove.myX;
    int nextTileY = owner.GetCurrentTileY() + nextMove.myY;

    //TODO IsAtDestination; or how to deal without nextTile members
    if (true)
    {
        if (world.TileIsValid(nextTileX, nextTileY))
        {
            Vector2f destination(nextTileX * TILE_SIZE, nextTileY * TILE_SIZE);
            Vector2f direction = destination - owner.GetPosition();

            float distanceToMove = time * AVATAR_SPEED;

            if (distanceToMove > direction.Length())
            {
                owner.SetPosition(destination);
            }
            else
            {
                direction.Normalize();
                owner.AddPosition(direction * distanceToMove);
            }
        }
    }
}
