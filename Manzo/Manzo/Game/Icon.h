#pragma once
#include "..\Engine\GameObject.h"
#include "..\Engine/Engine.h"
#include "vec2.h"

class Icon : public GameObject
{
public:
    Icon(vec2 position, GLTexture* icon_texture);
    GameObjectTypes         Type() override { return GameObjectTypes::Icon; };
    std::string             TypeName() override { return "Icon"; };
    void                    Update(double dt);
    void                    Draw();
    const vec2&             GetPosition() { return GameObject::GetPosition(); };



private:
    DrawCall                draw_call;
    GLTexture*              texture; 
    mat3                    matrix;
};