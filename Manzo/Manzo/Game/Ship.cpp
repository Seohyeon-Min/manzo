#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>
#include "../Engine/MapManager.h"

Ship::Ship(vec2 start_position) :
    GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false)
{
    AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    skill = Engine::GetGameStateManager().GetGSComponent<Skillsys>();
    fuel = Maxfuel;
    FuelFlag = false;
    SetVelocity({ 0,0 });
}

void Ship::Update(double dt)
{
    //std::cout << (!set_dest && beat->GetIsOnBeat() && !move) << std::endl;
    GameObject::Update(dt);

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        SetDest();

        if (move) {
            Move(dt);
        }
        else {
            if (hit_with) {
                vec2 velocity = GetVelocity();  // ��ü�� �ӵ� ����
                float velocity_magnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
                if(velocity_magnitude < 20.f) SetVelocity(direction * skidding_speed);
                else SetVelocity(GetVelocity() * deceleration);
                if (!beat->GetIsOnBeat()) {
                    SetVelocity(direction * skidding_speed);
                    hit_with = false;
                    if (!clickable) { // wait for next beat
                        clickable = true;
                    }
                }
            }

        }
        FuelUpdate(dt);
        if (isCollidingWithReef && !IsTouchingReef())
        {
            isCollidingWithReef = false;
        }
    }
}


void Ship::Draw()
{
    CS230::GameObject::Draw();
}

void Ship::SetDest()
{
    if (clickable && !set_dest) {
        if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && beat->GetIsOnBeat()) {
            // Get mouse position relative to the center of the screen
            vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
            vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
            vec2 pos = mouse_pos - window;
            destination.x = pos.x;
            destination.y = pos.y;
            clickable = false;
            set_dest = true;
        }
    }

    if (set_dest) { // if clicked the destination
        if (!beat->GetIsOnBeat() && !ready_to_move && !move) { // wait for next beat
            ready_to_move = true;
        }
    }

    if (ready_to_move && beat->GetBeat()) { // move when its on next beat
        direction = { destination.x - (GetPosition().x), destination.y - (GetPosition().y) };
        direction = direction.Normalize();
        force = { (direction * speed) };
        move = true;
        set_dest = false;
        ready_to_move = false;
    }
}

void Ship::Move(double dt)
{
    SetVelocity(force);
    force *= deceleration;
    if (!beat->GetIsOnBeat()) {
        SetVelocity(direction * skidding_speed);
        if (!clickable) { // wait for next beat
            clickable = true;
        }
        move = false;
    }
}

vec2 GetPerpendicular(vec2 v) {
    return { -v.y, v.x };
}


bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Fish:
    case GameObjectTypes::Reef:
        return true;
        break;
    }

    return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Reef) {
        auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
        if (collision_edge == nullptr) {
            // maybe an error?
        }
        HitWithReef(collision_edge);
    }
}

void Ship::HitWithReef(CS230::RectCollision* collision_edge) {
    fuel -= HitDecFuel;

    vec2 edge_1 = collision_edge->GetCollidingEdge().first;
    vec2 edge_2 = collision_edge->GetCollidingEdge().second;

    vec2 wall_dir = { edge_2.x - edge_1.x, edge_2.y - edge_1.y };
    vec2 wall_perpendicular = GetPerpendicular(wall_dir);
    vec2 normal = wall_perpendicular.Normalize();

    vec2 velocity = GetVelocity(); 

    float incoming_speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    float dot_product_normal_velocity = velocity.x * normal.x + velocity.y * normal.y;
    if (dot_product_normal_velocity > 0) {
        normal = normal * -1.0f;
    }

    float dot_product = velocity.x * normal.x + velocity.y * normal.y;
    vec2 reflection = {
        velocity.x - 2 * dot_product * normal.x,
        velocity.y - 2 * dot_product * normal.y
    };

    SetPosition(GetPosition() +  -GetVelocity() * 0.007f);
    direction = reflection.Normalize();
    if (incoming_speed > 3300.f)  incoming_speed = 3300.f;
    if (incoming_speed < 150.f)  incoming_speed = 150.f;
    SetVelocity(direction * incoming_speed * 0.75f);
    SetPosition(GetPosition() + normal * 0.5f);

    //float correction_distance = 0.5f;
    //vec2 corrected_position = GetPosition() + normal * correction_distance;
    //SetPosition(corrected_position);

    move = false;
    hit_with = true;
}

//for fuel
void Ship::FuelUpdate(double dt)
{

    bool Reduce = false;

    if (fuel <= 0)
    {
        FuelFlag = true;
    }

    if (FuelFlag == false)
    {
        fuelcounter += dt;

        if (fuelcounter >= 1) // Decreased fuel per second
        {
            fuel -= baseDecfuel;
            fuelcounter = 0;
        }

        if (move == true)
        {
            Reduce = true;
            if (Reduce == true)
            {
                fuel -= MoveDecfuel;
                Reduce = false;
            }
        }

        //std::cout << "Fuel: " << fuel << std::endl;
    }

    if (FuelFlag == true)
    {
        IsFuelZero();
    }
}

void Ship::SetMaxFuel(double input)
{
    Maxfuel = input;
}



bool Ship::IsTouchingReef()
{
    if (isCollidingWithReef == true)
    {
        return true;
    }
    return false;
}

bool Ship::IsFuelZero()
{
    return FuelFlag;
}