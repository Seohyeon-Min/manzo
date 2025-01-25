#include "mouse.h"
#include "..\Engine\Input.h"
#include "BeatSystem.h"
#include "Particles.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



Mouse::Mouse() : GameObject({}),
mouse_position({ 0, 0 }),
trails(trail_length, { vec2(0, 0), 1.0f })
{
    AddGOComponent(new Sprite("assets/images/mouse.spt", this));
    SetCameraFixed(true);
}


void Mouse::Update(double dt) 
{
    GameObject::Update(dt);

    mouse_position.x = Engine::GetInput().GetMousePos().mouseCamSpaceX;
    mouse_position.y = Engine::GetInput().GetMousePos().mouseCamSpaceY;
    SetPosition(mouse_position);
    FollowMouse(mouse_position);

    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        scale = scale_big;
    }

    if (scale <= basic_scale) scale = basic_scale;
    else scale -= (float)dt * scale_decrease_factor;
}

void Mouse::Draw(DrawLayer drawlayer)
{
    DrawLaserCurve();
    DrawMouseCursor();
}

void Mouse::FollowMouse(const vec2& mouse_position) {
    UpdateTrail(mouse_position);
}

void Mouse::UpdateTrail(const vec2& new_position) {
    trails.push_back({ new_position, 1.0f });

    if (trails.size() > trail_length) {
        trails.erase(trails.begin());
    }
}

void Mouse::DrawMouseCursor()
{
    //SetFiltering(GLTexture::Linear);
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader()
    };

    draw_call.settings.is_camera_fixed = true;
    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = DrawLayer::DrawUI;

    GameObject::Draw(draw_call);
}


void Mouse::DrawLaserCurve() {
    if (trails.size() > 1) {
        float line_width = 5.0f;
        for (size_t i = trails.size() - 1; i > 0; --i) {
            vec2 previous_point = trails[i].position;
            vec2 current_point = trails[i - 1].position;
            line_width *= decrease_factor;
            if (line_width <= 0.001f) {
                line_width = 0.0001f;
            }

            LineDrawCallPro draw_call{ previous_point, current_point, { 255,255,255 }, line_width, 255.0f, nullptr, false };
            draw_call.settings.is_camera_fixed = true;

            Engine::GetRender().AddDrawCall(std::make_unique<LineDrawCallPro>(draw_call));

            previous_point = current_point;
        }
    }
}

