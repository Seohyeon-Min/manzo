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
        //std::cout << "x: " << this->GetPosition().x << "y: " << this->GetPosition().y << std::endl;

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
        if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && beat->GetIsOnBeat() ) {
            // Get mouse position relative to the center of the screen
            vec2 window = { Engine::window_width/2, Engine::window_height/2 };
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
            //std::cout << "ready_to_move" << std::endl;
            ready_to_move = true;
        }
    }

    if (ready_to_move && beat->GetBeat()) { // move when its on next beat
        initialPosition = GetPosition();
        //std::cout << "move" << std::endl;
        move = true;
        set_dest = false;
        ready_to_move = false;
    }
}

void Ship::Move(double dt)
{ // there is a bug that if destination is too short, ship doesn't move any more

    double distanceMoved_SQUARED = (double)pow(GetPosition().x - initialPosition.x, 2) + pow(GetPosition().y - initialPosition.y, 2);


    vec2 direction = { destination.x - (GetPosition().x),
                         destination.y - (GetPosition().y) };
    //vec2 direction = { destination.x - (GetPosition().x - (float)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().x),
                //			 destination.y - (GetPosition().y - (float)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().y)};
    float magnitude = (float)sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude != 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }


    if (distanceMoved_SQUARED >= totalDistanceToMove_SQUARED) { // stop

        SetVelocity({ 0, 0 });
        currentSpeed = initialSpeed;
        if (beat->GetIsOnBeat() && !clickable) { // wait for next beat
            //std::cout << "clickable" << std::endl;
            clickable = true;
        }
        if(beat->GetBeat()){
            //std::cout << "move_false" << std::endl;
            move = false;
        }

  
    }
    else {
        SetVelocity({ direction.x * float(initialSpeed), direction.y * float(initialSpeed)}); //move if left
        if (currentSpeed > 0) {
            currentSpeed -= (float)(deceleration);
            if (currentSpeed < 0)
            {
                currentSpeed = 0;
            }
        }
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
        SetVelocity({ 0, 0 });
        if (!isCollidingWithReef) {
            HitWithReef();
            isCollidingWithReef = true;
        }
        break;
    }
}

bool Ship::IsShipUnder()
{
    if (this->GetPosition().y > 170)
    {
        return true;
    }
    return false;
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