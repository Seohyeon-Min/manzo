#include "mouse.h"
#include "..\Engine\Input.h"
#include "Particles.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



Mouse::Mouse() : mouse_cursor(nullptr),
mouse_position({ 0, 0 }),
trails(trail_length, { vec2(0, 0), 1.0f }) {
    mouse_cursor = Engine::GetTextureManager().Load("assets/images/mouse.png");
    mouse_cursor->SetFiltering(GLTexture::Linear);
}


void Mouse::Update(double dt) {
    mouse_position = Engine::GetInput().GetMousePosition();
    FollowMouse(mouse_position);
    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        scale = scale_big;
    }

    if (scale <= basic_scale) scale = basic_scale;
    else scale -= (float)dt * scale_decrease_factor;
}

void Mouse::AddDrawCall()
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

            Engine::GetRender().AddDrawCall(previous_point, current_point, { 255,255,255 }, line_width, 255.0f, nullptr, false);

            previous_point = current_point;
        }
    }
}

void Mouse::DrawMouseCursor()
{
    DrawSettings settings;
    settings.is_UI = true;
    settings.do_blending = true;

    pos_matrix = mat3::build_translation({ mouse_position.x - Engine::window_width / 2 , mouse_position.y - Engine::window_height / 2 }) * mat3::build_scale(scale);

    draw_call = {
        mouse_cursor,                       // Texture to draw
        &pos_matrix,                          // Transformation matrix
        Engine::GetShaderManager().GetDefaultShader(), // Shader to use
        nullptr,
        settings
    };

    Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}
