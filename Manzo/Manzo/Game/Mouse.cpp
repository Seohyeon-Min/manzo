#include "mouse.h"
#include "..\Engine\Input.h"
#include "Particles.h"
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



Mouse::Mouse() {
}


void Mouse::Update(double dt) {
    //vec2 mouse_pos = {(float) Engine::GetInput().GetMousePos().mouseWorldSpaceX, (float)Engine::GetInput().GetMousePos().mouseWorldSpaceY };
    vec2 mouse_pos = Engine::GetInput().GetMousePosition();
    FollowMouse(mouse_pos);
}

void Mouse::FollowMouse(const vec2& mouse_position) {
    UpdateTrail(mouse_position);
    DrawLaserCurve();
}


void Mouse::UpdateTrail(const vec2& new_position) {
    trails.push_back({ new_position, 1.0f });

    if (trails.size() > trail_length) {
        trails.erase(trails.begin());
    }
}

void Mouse::DrawLaserCurve() {
    if (trails.size() > 1){
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