#pragma once
#include "..\Engine\Particle.h"
#include "..\Engine\GameObjectManager.h"
#include "..\Engine\Component.h"
#include "../Engine/UI.h"

struct Trail {
    vec2 position; 
    float alpha; 
};

class Mouse : public GameObject {
public:
    Mouse();
    GameObjectTypes Type() override { return GameObjectTypes::Mouse; }
    std::string TypeName() override { return "Mouse"; }
    void Update([[maybe_unused]] double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    void FollowMouse(const vec2& mouse_position);
    void ExplodeOnClick(const vec2& click_position);
    void UpdateTrail(const vec2& new_position);
    void DrawMouseCursor();
    void DrawLaserCurve();

    mat3 pos_matrix{};
    vec2 mouse_position{};
    std::vector<Trail> trails;
    vec2 first_point{};
    size_t trail_length = 10;

    float scale = 0.6f;
    const float basic_scale = 0.6f;
    const float scale_big = 1.2f;
    const float scale_decrease_factor = 3.f;
    const float decrease_factor = 0.75f;
};