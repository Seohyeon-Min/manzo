#include "mouse.h"
#include "..\Engine\Input.h"
#include "Particles.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 잔향을 저장할 구조체
struct Trail {
    vec2 position; // 잔향의 위치
    float alpha;   // 투명도
};

// 잔향을 저장할 벡터
std::vector<Trail> trails;

Mouse::Mouse() {}

void Mouse::Update(double dt) {
    vec2 mouse_pos = Engine::GetInput().GetMousePosition();
    UpdateTrail(mouse_pos); // 업데이트 시 새로운 위치를 잔향에 추가
    FollowMouse(mouse_pos);
}

void Mouse::UpdateTrail(const vec2& new_position) {
    // 새 위치를 잔향으로 추가
    trails.push_back({ new_position, 1.0f }); // 초기 투명도는 1.0f

    // 잔향의 투명도 감소
    for (auto& trail : trails) {
        trail.alpha -= 0.01f; // 시간에 따라 투명도 감소
        if (trail.alpha < 0.0f) {
            trail.alpha = 0.0f; // 최소 투명도는 0
        }
    }

    // 잔향의 크기가 100개를 넘으면 가장 오래된 것을 제거
    if (trails.size() > 100) {
        trails.erase(trails.begin());
    }
}

void Mouse::FollowMouse(const vec2& mouse_position) {
    if (Engine::GetInput().IsMouseMoving()) {
        std::vector<vec2> visible_trails;

        // 잔향에서 알파가 0보다 큰 위치를 추출
        for (const auto& trail : trails) {
            if (trail.alpha > 0.0f) {
                visible_trails.push_back(trail.position);
            }
        }

        // 최소 4개의 포인트가 필요하므로 보충
        while (visible_trails.size() < 4) {
            visible_trails.push_back(mouse_position); // 현재 마우스 위치를 추가
        }

        if(visible_trails.size() >=4)
        DrawLaserCurve(visible_trails); // 가시적인 잔향으로 곡선 그리기
    }
}

void Mouse::ExplodeOnClick(const vec2& click_position) {
    // 클릭 시 폭발 처리
    // TODO: 구현
}

void Mouse::DrawLaserCurve(const std::vector<vec2>& control_points) {
    int segment_count = 50; // 곡선을 나눌 선분 개수
    vec2 previous_point = CalculateBezierPoint(0, control_points);

    for (int i = 1; i <= segment_count; ++i) {
        float t = static_cast<float>(i) / segment_count;
        vec2 current_point = CalculateBezierPoint(t, control_points);

        // 선분 그리기
        Engine::GetRender().AddDrawCall(previous_point, current_point, {255,255,255});

        previous_point = current_point;
    }
}

vec2 Mouse::CalculateBezierPoint(float t, const std::vector<vec2>& control_points) {
    if (control_points.size() != 4) {
        throw std::invalid_argument("Bezier curve requires exactly 4 control points.");
    }

    const vec2& P0 = control_points[0];
    const vec2& P1 = control_points[1];
    const vec2& P2 = control_points[2];
    const vec2& P3 = control_points[3];

    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;

    // 베지어 곡선의 계산
    vec2 point = (uu * u) * P0;           // (1 - t)^3 * P0
    point += (3 * uu * t) * P1;           // 3 * (1 - t)^2 * t * P1
    point += (3 * u * tt) * P2;           // 3 * (1 - t) * t^2 * P2
    point += (tt * t) * P3;               // t^3 * P3

    return point;
}