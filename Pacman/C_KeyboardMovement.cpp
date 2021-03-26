#include "C_KeyboardMovement.h"
#include "Input.h"
#include "World.h"
#include "GameEntity.h"
#include "Constants.h"

C_KeyboardMovement::C_KeyboardMovement(GameEntity& owner, 
    Input& input, const World& world) : 
    Component(owner), 
    moveSpeed(AVATAR_SPEED), 
    input(input),
    world(world),
    nextMovement(-1.f, 0.f) {
    myCurrentTileX = myNextTileX = owner.GetPosition().myX / TILE_SIZE;
    myCurrentTileY = myNextTileY = owner.GetPosition().myY / TILE_SIZE;
}

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

    Move(time);
}

void C_KeyboardMovement::Move(float time)
{
    int nextTileX = myCurrentTileX + nextMovement.myX;
    int nextTileY = myCurrentTileY + nextMovement.myY;

    if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
    {
        if (world.TileIsValid(nextTileX, nextTileY))
        {
            myNextTileX = nextTileX;
            myNextTileY = nextTileY;
        }
    }

    Vector2f destination(myNextTileX * TILE_SIZE, myNextTileY * TILE_SIZE);
    Vector2f direction = destination - owner.GetPosition();

    float distanceToMove = time * AVATAR_SPEED;

    if (distanceToMove > direction.Length())
    {
        owner.SetPosition(destination);
        myCurrentTileX = myNextTileX;
        myCurrentTileY = myNextTileY;
    }
    else
    {
        direction.Normalize();
        owner.AddPosition(direction * distanceToMove);
    }
}
