#pragma once

#include "../Engine/GameObject.h"
#include "Ship.h"

class Module : public GameObject
{
public:
    Module(vec2 start_position, Ship* ship);
    GameObjectTypes Type() override { return GameObjectTypes::Module; }
    std::string TypeName() override { return "Module"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

private:
    Ship* ship;

    
    /*class State_First_Module : public State 
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "First Module"; }

    private:
        vec2 dash_destination;
    };

    State_First_Module first_module;*/
};