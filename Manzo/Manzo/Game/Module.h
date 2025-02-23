#pragma once

#include "../Engine/GameObject.h"
#include "Ship.h"

class Module : public GameObject
{
public:
    Module();
    Module(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Module; }
    std::string TypeName() override { return "Module"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

    bool IsFirstSetted() { return first_module_setted; }
    bool IsSecondSetted() { return second_module_setted; }
    bool IsThirdSetted() { return third_module_setted; }

private:
    bool first_module_setted = false;
    bool second_module_setted = false;
    bool third_module_setted = false;
};

class FirstModule : public Module
{
public:
    FirstModule(Ship* ship);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

private:
    Ship* ship;
};