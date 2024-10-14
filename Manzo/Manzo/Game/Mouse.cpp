#include "mouse.h"
#include "..\Engine\Input.h"
#include "Particles.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ������ ������ ����ü
struct Trail {
    vec2 position; // ������ ��ġ
    float alpha;   // ����
};

// ������ ������ ����
std::vector<Trail> trails;

Mouse::Mouse() {}

void Mouse::Update(double dt) {
    vec2 mouse_pos = Engine::GetInput().GetMousePosition();
    UpdateTrail(mouse_pos); // ������Ʈ �� ���ο� ��ġ�� ���⿡ �߰�
    FollowMouse(mouse_pos);
}

void Mouse::UpdateTrail(const vec2& new_position) {
    // �� ��ġ�� �������� �߰�
    trails.push_back({ new_position, 1.0f }); // �ʱ� ������ 1.0f

    // ������ ���� ����
    for (auto& trail : trails) {
        trail.alpha -= 0.01f; // �ð��� ���� ���� ����
        if (trail.alpha < 0.0f) {
            trail.alpha = 0.0f; // �ּ� ������ 0
        }
    }

    // ������ ũ�Ⱑ 100���� ������ ���� ������ ���� ����
    if (trails.size() > 100) {
        trails.erase(trails.begin());
    }
}

void Mouse::FollowMouse(const vec2& mouse_position) {
    if (Engine::GetInput().IsMouseMoving()) {
        std::vector<vec2> visible_trails;

        // ���⿡�� ���İ� 0���� ū ��ġ�� ����
        for (const auto& trail : trails) {
            if (trail.alpha > 0.0f) {
                visible_trails.push_back(trail.position);
            }
        }

        // �ּ� 4���� ����Ʈ�� �ʿ��ϹǷ� ����
        while (visible_trails.size() < 4) {
            visible_trails.push_back(mouse_position); // ���� ���콺 ��ġ�� �߰�
        }

        if(visible_trails.size() >=4)
        DrawLaserCurve(visible_trails); // �������� �������� � �׸���
    }
}

void Mouse::ExplodeOnClick(const vec2& click_position) {
    // Ŭ�� �� ���� ó��
    // TODO: ����
}

void Mouse::DrawLaserCurve(const std::vector<vec2>& control_points) {
    int segment_count = 50; // ��� ���� ���� ����
    vec2 previous_point = CalculateBezierPoint(0, control_points);

    for (int i = 1; i <= segment_count; ++i) {
        float t = static_cast<float>(i) / segment_count;
        vec2 current_point = CalculateBezierPoint(t, control_points);

        // ���� �׸���
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

    // ������ ��� ���
    vec2 point = (uu * u) * P0;           // (1 - t)^3 * P0
    point += (3 * uu * t) * P1;           // 3 * (1 - t)^2 * t * P1
    point += (3 * u * tt) * P2;           // 3 * (1 - t) * t^2 * P2
    point += (tt * t) * P3;               // t^3 * P3

    return point;
}