#pragma once
#include <vec2.h>
#include "Component.h"
#include "GameObjectManager.h"
#include "Engine.h"

class RayCasting : public Component
{
public:
    vec2 m_end;
    vec2 g_mouse_pos;

    RayCasting(float x, float y);
    void calc_hit(vec2 wall1, vec2 wall2);
    void reset();

private:
    vec2 m_relative_end;
};