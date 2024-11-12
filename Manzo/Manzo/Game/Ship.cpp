#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>

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
    std::cout << force.x << std::endl;
    if (!beat->GetIsOnBeat()) {
        SetVelocity(direction * skidding_speed);
        if (!clickable) { // wait for next beat
            clickable = true;
        }
        move = false;
    }
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
    switch (other_object->Type()) {

    case GameObjectTypes::Reef:
        IsTouchingReef();
        if (!isCollidingWithReef) {
            HitWithReef();
            isCollidingWithReef = true;
        }
        break;
    }
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

void Ship::HitWithReef()
{
    fuel -= HitDecFuel;
    std::cout << "Collision with Reef!" << std::endl;
}