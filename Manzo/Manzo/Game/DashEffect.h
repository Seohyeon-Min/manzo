#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "Ship.h"

class DashEffect : public CS230::GameObject {
public:
    DashEffect() :
        GameObject({}), effect_timer(new CS230::Timer(effect_time)){
        AddGOComponent(new CS230::Sprite("assets/images/dash_effect.spt", this));
        ship = Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->GetGOComponent<Ship>();
        vec2 dir = ship->GetVelocity().Normalize();
        float angle_radians = std::atan2(dir.y, dir.x);
        SetPosition(ship->GetPosition());
        SetRotation(angle_radians);
        AddGOComponent(effect_timer);
    }
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "DashEffect Particle"; }
    void Update(double dt) override {
        GameObject::Update(dt);
        if (effect_timer->Remaining() == 0) Destroy();
    }
private:
    Ship* ship;
    const double effect_time = 0.3;
    CS230::Timer* effect_timer;
};