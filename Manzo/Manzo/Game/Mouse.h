#pragma once
#include "..\Engine\Particle.h"
#include "..\Engine\GameObjectManager.h"
#include "..\Engine\Component.h"



// 잔향을 저장할 구조체
struct Trail {
    vec2 position; // 잔향의 위치
    float alpha;   // 투명도
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
