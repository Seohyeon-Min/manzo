#pragma once
#include "..\Engine\Particle.h"
#include "..\Engine\GameObjectManager.h"
#include "..\Engine\Component.h"



// ������ ������ ����ü
struct Trail {
    vec2 position; // ������ ��ġ
    float alpha;   // ����
};

class Mouse : public CS230::Component {
public:
    Mouse();
    void Update(double dt) override;
    void FollowMouse(const vec2& mouse_position);
    void ExplodeOnClick(const vec2& click_position);

private:
    void UpdateTrail(const vec2& new_position);
    void DrawLaserCurve();
    void DrawMouseCursor(const vec2& mouse_position);

    std::vector<Trail> trails;
    vec2 first_point;
    size_t trail_length = 10;
    const float decrease_factor = 0.75f;
    GLTexture* mouse_cursor;
};
