#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>
#include "Reef.h"


Ship::Ship(vec2 start_position) :
    GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false)
{
    AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    SetVelocity({ 0,0 });
}

void Ship::Update(double dt)
{
    GameObject::Update(dt);
    SetDest();
    if (move) {
        Move(dt);
    }
}

void Ship::Draw()
{
    GameObject::Draw();
}

void Ship::SetDest()
{
    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && !set_dest && beat->GetIsOnBeat() && !move) {
        // 마우스 위치를 화면의 중심 기준으로 가져옴
        destination.x = Engine::GetInput().GetMousePosition().x;
        destination.y = Engine::GetInput().GetMousePosition().y;

        set_dest = true;
    }

    if (set_dest) { // 목적지가 설정되면
        if (!beat->GetIsOnBeat()) { // 다음 비트를 기다림
            ready_to_move = true;
        }
    }

    if (ready_to_move && beat->GetBeat()) { // 다음 비트에서 이동 시작
        initialPosition = GetPosition();

        move = true;
        set_dest = false;
        ready_to_move = false;
    }
}

void Ship::Move(double dt)
{
    // 목적지가 너무 가까우면 배가 멈추는 버그가 있음
    float distanceMoved = (float)sqrt(pow(GetPosition().x - initialPosition.x, 2) + pow(GetPosition().y - initialPosition.y, 2));

    vec2 direction = { destination.x - (GetPosition().x),
                       destination.y - (GetPosition().y) };

    float magnitude = (float)sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude != 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    float totalDistanceToMove = 150.0f;

    if (distanceMoved >= totalDistanceToMove) { // 멈춤
        SetVelocity({ 0, 0 });
        currentSpeed = initialSpeed;
        move = false;
    }
    else {
        if (currentSpeed > 0) {
            currentSpeed -= (float)(deceleration);
            if (currentSpeed < 0) currentSpeed = 0;
        }

        SetVelocity({ direction.x * currentSpeed, direction.y * currentSpeed }); // 이동
    }
}

bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Reef:
    case GameObjectTypes::Fish:
        return true;
    default:
        return false;
    }
}


vec2 GetPerpendicular(vec2 v) {
    return { -v.y, v.x }; 
}

vec2 Ship::GetNormal(vec2 reefPosition1, vec2 reefPosition2) {
    vec2 wallDirection = { reefPosition2.x - reefPosition1.x, reefPosition2.y - reefPosition1.y };

    vec2 normal = GetPerpendicular(wallDirection);

    float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y);
    if (magnitude != 0) {
        normal.x /= magnitude;
        normal.y /= magnitude;
    }

    return normal;
}

void Ship::ResolveCollision(GameObject* other_object)
{ 
    if(other_object->GetGOComponent<CS230::RectCollision>() != nullptr){
            vec2 CollisionReefpoint1 = other_object->GetGOComponent<CS230::RectCollision>()->CollidingSide_1;
            vec2 CollisionReefpoint2 = other_object->GetGOComponent<CS230::RectCollision>()->CollidingSide_2;
    
        if (other_object->Type() == GameObjectTypes::Reef) {
        
            Reef* reef = dynamic_cast<Reef*>(other_object);
            vec2 normal = GetNormal(CollisionReefpoint1, CollisionReefpoint2);
            vec2 velocity = GetVelocity();

            float dotProduct = velocity.x * normal.x + velocity.y * normal.y;
            vec2 reflection = {
                velocity.x - 2 * dotProduct * normal.x,
                velocity.y - 2 * dotProduct * normal.y
            };

            SetVelocity(reflection);

            currentSpeed = initialSpeed;
            move = true; 
        }
    }
}
