

#include "../Engine/Engine.h"
#include "../Engine/ShowCollision.h"

#include "States.h"
#include "Background.h"
#include "Ship.h"
#include "Mode2.h"

#include "Mode1.h"


#include <iostream>     // for debug

Mode2::Mode2(){}

void Mode2::Load() {

#ifdef _DEBUG
    AddGSComponent(new CS230::ShowCollision());
#else
#endif
    // compenent
    AddGSComponent(new CS230::GameObjectManager());
    //AddGSComponent(new Background());
    //GetGSComponent<Background>()->Add("assets/images/temp_back.png", 0.25);

    //// ship
    ship_ptr = new Ship({ 0, 0 });
    GetGSComponent<CS230::GameObjectManager>()->Add(ship_ptr);

    //// camera
    AddGSComponent(new CS230::Camera({ {1280 / 2 , 720 / 2 }, {1280 / 2, 720 / 2 } }));
    vec2 playerPosition = ship_ptr->GetPosition();
    GetGSComponent<CS230::Camera>()->SetPosition({ playerPosition.x - 1280 / 2, playerPosition.y - 720 / 2 });

}

void Mode2::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<CS230::GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<CS230::Camera>()->Update(ship_ptr->GetPosition());

}

void Mode2::Draw() {
    GetGSComponent<CS230::GameObjectManager>()->DrawAll();
}

void Mode2::Unload() {

    GetGSComponent<CS230::GameObjectManager>()->Unload();
    ClearGSComponents();
}