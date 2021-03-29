#include "C_KeyboardMovement.h"
#include "C_Animation.h"
#include "Input.h"
#include "World.h"
#include "GameEntity.h"

C_KeyboardMovement::C_KeyboardMovement(GameEntity& owner, Input* input, const World* world, float moveSpeed) : 
    Component(owner), 
    input{ input },
    world{ world },
    moveSpeed(moveSpeed) {}

void C_KeyboardMovement::SetMoveSpeed(float moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

void C_KeyboardMovement::Awake()
{
    animation = owner.GetComponent<C_Animation>();
}

void C_KeyboardMovement::Update(float time)
{
    Vector2f possibleMovement;
    if (input->IsKeyDown(Input::Key::Up))
        possibleMovement = Vector2f(0.f, -1.f);
    else if (input->IsKeyDown(Input::Key::Down))
        possibleMovement = Vector2f(0.f, 1.f);
    else if (input->IsKeyDown(Input::Key::Left))
        possibleMovement = Vector2f(-1.f, 0.f);
    else if (input->IsKeyDown(Input::Key::Right))
        possibleMovement = Vector2f(1.f, 0.f);

    if (possibleMovement == Vector2f(0.f, 0.f))
        possibleMovement = nextMovement;
    Move(time, possibleMovement);
}

void C_KeyboardMovement::Start()
{
    nextMovement = Vector2f(-1.f, 0.f);
    myCurrentTileX = myNextTileX = owner.GetPosition().myX / TILE_SIZE;
    myCurrentTileY = myNextTileY = owner.GetPosition().myY / TILE_SIZE;
}

void C_KeyboardMovement::Move(float time, Vector2f possibleMove)
{
    int nextTileX = myCurrentTileX + possibleMove.myX;
    int nextTileY = myCurrentTileY + possibleMove.myY;

    if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
    {
        if (world->TileIsValid(nextTileX, nextTileY))
        {
            myNextTileX = nextTileX;
            myNextTileY = nextTileY;
            nextMovement = possibleMove;
            auto state = animation->GetAnimationState();
            if (nextMovement.myX == 1)
            {
                if (animation && state != AnimationState::GoingRight)
                    animation->SetAnimationState(AnimationState::GoingRight);
            }
            else if (nextMovement.myX == -1)
            {
                if (animation && state != AnimationState::GoingLeft)
                    animation->SetAnimationState(AnimationState::GoingLeft);
            }
            else if (nextMovement.myY == 1)
            {
                if (animation && state != AnimationState::GoingDown)
                    animation->SetAnimationState(AnimationState::GoingDown);
            }
            else if (nextMovement.myY == -1)
            {
                if (animation && state != AnimationState::GoingUp)
                    animation->SetAnimationState(AnimationState::GoingUp);
            }
        }
    }

    Vector2f destination(myNextTileX * TILE_SIZE, myNextTileY * TILE_SIZE);
    Vector2f direction = destination - owner.GetPosition();

    float distanceToMove = time * moveSpeed;

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
