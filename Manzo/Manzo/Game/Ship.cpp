#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>
#include "../Engine/MapManager.h"

Ship::Ship(vec2 start_position) :
    GameObject(start_position), move(false)
{
    AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
    beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
    skill = Engine::GetGameStateManager().GetGSComponent<Skillsys>();
    fuel = Maxfuel;
    FuelFlag = false;
    SetVelocity({ 0,0 });

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        current_state = &state_idle;
        current_state->Enter(this);
    }
}


void Ship::State_Idle::Enter(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    if (ship->GetVelocity() != vec2{})
        ship->SetVelocity(ship->direction * skidding_speed);
}
void Ship::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    //get mouse pos and move to there
    Ship* ship = static_cast<Ship*>(object);

    vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
    vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
    vec2 pos = mouse_pos - window;

    vec2 destination = pos;
    vec2 direction = destination - ship->GetPosition();
    float distance = direction.Length(); 
    direction = direction.Normalize();

    float force_multiplier = 0.0f;
    float min_distance = 50.0f; 
    float max_distance = 200.0f; 

    if (distance <= min_distance) {
        force_multiplier = 0.0f; 
    }
    else if (distance >= max_distance) {
        force_multiplier = 1.0f; 
    }
    else {
        force_multiplier = (distance - min_distance) / (max_distance - min_distance);
    }

    vec2 force = direction * skidding_speed * force_multiplier;
    ship->SetVelocity(force);
}
void Ship::State_Idle::CheckExit(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    //if (ship->hit_with) {
    //    ship->change_state(&ship->state_hit);
    //}
    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && ship->beat->GetIsOnBeat()) {
        // Get mouse position relative to the center of the screen
        vec2 window = { Engine::window_width / 2, Engine::window_height / 2 };
        vec2 mouse_pos = { (float)Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
        vec2 pos = mouse_pos - window;
        ship->destination.x = pos.x;
        ship->destination.y = pos.y;
        ship->direction = { ship->destination.x - (ship->GetPosition().x), ship->destination.y - (ship->GetPosition().y) };
        ship->direction = ship->direction.Normalize();
        ship->force = { (ship->direction * speed) };
        ship->change_state(&ship->state_move);
    }
}


void Ship::State_Move::Enter(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    ship->move = true;
}
void Ship::State_Move::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) { }
void Ship::State_Move::FixedUpdate([[maybe_unused]] GameObject* object, [[maybe_unused]] double fixed_dt) {
    Ship* ship = static_cast<Ship*>(object);
    ship->FuelUpdate(fixed_dt);
    ship->SetVelocity(ship->force);
    float base_dt = 1.0f / 240.f;
    float adjusted_deceleration = (float)pow(deceleration, fixed_dt / base_dt);
    ship->force *= adjusted_deceleration;
}
void Ship::State_Move::CheckExit(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    if (ship->hit_with) {
        ship->change_state(&ship->state_hit);
    }
    if (!ship->beat->GetIsOnBeat()) {
        ship->move = false;
        ship->change_state(&ship->state_idle);
    }
}

std::vector<vec2> spline_points;

void Ship::State_Hit::Enter(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    std::cout << "state_hit_enter~~~~~~~~~~~~~\n";
    //ship->HitWithReef(ship->normal);
}
void Ship::State_Hit::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {
    Ship* ship = static_cast<Ship*>(object);
}
void Ship::State_Hit::FixedUpdate(GameObject* object, double fixed_dt) {}
void Ship::State_Hit::CheckExit(GameObject* object) {
    Ship* ship = static_cast<Ship*>(object);
    std::cout << "state_hit_exit~~~~~~~~~~~~~\n";
    ship->change_state(&ship->state_idle);
}


void Ship::Update(double dt)
{
    //if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)) 
    //    std::cout << "move!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    //std::cout << (!hit_with ? "can collide" : "no") << std::endl;
    //std::cout << (!set_dest && beat->GetIsOnBeat() && !move) << std::endl;
    GameObject::Update(dt);
    CS230::RectCollision* collider = GetGOComponent<CS230::RectCollision>();

    // World Boundary
    if (collider->WorldBoundary_rect().Left() < Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x - 640) {
        UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x - 640 - collider->WorldBoundary_rect().Left(), 0 });
        SetVelocity({ 0, GetVelocity().y });
    }
    if (collider->WorldBoundary_rect().Right() > Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x + 640) {
        UpdatePosition({ Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().x + 640 - collider->WorldBoundary_rect().Right(), 0 });
        SetVelocity({ 0, GetVelocity().y });
    }
    if (collider->WorldBoundary_rect().Bottom() < Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y - 360) {
        UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y - 360 - collider->WorldBoundary_rect().Bottom() });
        SetVelocity({ GetVelocity().x, 0 });
    }
    if (collider->WorldBoundary_rect().Top() > Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y + 360) {
        UpdatePosition({ 0, Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->GetPosition().y + 360 - collider->WorldBoundary_rect().Top() });
        SetVelocity({ GetVelocity().x, 0 });
    }

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {

        if (isCollidingWithReef && !IsTouchingReef())
        {
            isCollidingWithReef = false;
        }
    }
}

void Ship::FixedUpdate(double fixed_dt) {
    GameObject::FixedUpdate(fixed_dt);
}


void Ship::Draw(DrawLayer drawlayer) {
    CS230::GameObject::Draw(DrawLayer::DrawLast);
}


bool Ship::CanCollideWith(GameObjectTypes other_object)
{
    switch (other_object) {
    case GameObjectTypes::Fish:
    case GameObjectTypes::Rock:
        if (!hit_with) return true;
        else return false;
        break;
    }

    return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Rock) {

        if (GetVelocity().Length() <= skidding_speed + 30.f) { // if it was skidding, don't reflect
            vec2 smallCorrection = -GetVelocity().Normalize();
            UpdatePosition(smallCorrection);
            return;
        }

        std::pair<vec2, vec2> edge = other_object->GetGOComponent<CS230::MAP_SATCollision>()->GetCollidingEdge();
        vec2 pointA = edge.first;
        vec2 pointB = edge.second;

        vec2 edgeVector = pointB - pointA;
        vec2 normalVector = { -edgeVector.y, edgeVector.x };


        SetVelocity({});
        hit_with = true;
        std::cout << "collide~~~~~~~~~~~~~\n";
        normal = normalVector.Normalize();
        HitWithReef(normal);
    }
}

bool IsNearPoint(const vec2& object_position, const vec2& point, float threshold) {
    return (object_position - point).Length() <= threshold;
}


void Ship::HitWithReef(vec2 normal) {
    std::cout << "hit_wit_reef~~~~~~~~~~~~~\n";
    fuel -= HitDecFuel;

    if (fuel < 0.0f) {
        fuel = 0.0f;
    }

    vec2 velocity = GetVelocity();

    float dot_product = velocity.x * normal.x + velocity.y * normal.y;

    vec2 reflection = {
        velocity.x - 2 * dot_product * normal.x,
        velocity.y - 2 * dot_product * normal.y
    };

    float incoming_speed = velocity.Length();

    SetVelocity(reflection.Normalize() * incoming_speed * 0.35f);

    move = false;

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
    fuel -= HitDecFuel;
    //std::cout << "Collision with Reef!" << std::endl;

}

bool Ship::IsShipUnder()
{
    if (this->GetPosition().y > 180)
    {
        return true;
    }
    return false;
}