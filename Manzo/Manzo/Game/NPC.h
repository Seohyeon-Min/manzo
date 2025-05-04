#include "Player.h"
#include "../Engine/Timer.h"
#pragma once

class NPC : public Player {
public:
    NPC(vec2 start_position = { -100, -115 });
    GameObjectTypes Type() override { return GameObjectTypes::Player; }
    std::string TypeName() override { return "NPC"; }
    void Update(double dt) override;
    bool Walk();
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    Timer* walk_timer;
    bool has_run = false;
};