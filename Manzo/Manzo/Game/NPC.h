#pragma once
#include "..\Engine\GameObject.h"
#include "../Engine/Timer.h"
constexpr vec2 start_pos_NPC = { -600,-115 };

class NPC : public GameObject {
public:
    NPC(vec2 start_position = start_pos_NPC);
    GameObjectTypes Type() override { return GameObjectTypes::Player; }
    std::string TypeName() override { return "NPC"; }
    void Update(double dt) override;
    void FixedUpdate(double fixed_dt) override;
    void Walk();
    bool IsWalking() { return is_walking; }
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    bool is_walking = false;
    RealTimeTimer* walk_timer;
    float walking_speed = 90.f;
};