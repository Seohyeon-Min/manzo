#pragma once

#include "../Engine/Component.h"
#include "../Engine/Icon.h"

class Dragging : public Component
{
public:
    Dragging(Icon& object);
    ~Dragging();
    void Update(double dt) override;

private:
    Icon& object;
    vec2 icon_first_pos;
};