#pragma once

#include "Component.h"
#include "GameObject.h"

class Dragging : public Component
{
public:
    Dragging(GameObject& object);
    void Update(double dt) override;

private:
    GameObject& object;
    vec2 icon_first_pos;
    //GameObject& mouse;
};