#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "Particles.h"
#include "Ship.h"
#include "../Engine/Sprite.h"


class Effect : public GameObject {
public:
    Effect(vec2 position, double effect_time);
    virtual ~Effect();
    GameObjectTypes Type() override { return GameObjectTypes::Fish; }
    std::string TypeName() override { return "Fish"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

protected:
    virtual void SetAlpha(const GLShader* shader);

    Timer* effect_timer;
    double effect_time;
};

class DashEffect : public Effect {
public:
    DashEffect();
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

private:
    Ship* ship;
    vec2 force;
    static constexpr float speed = 1000.0f;
    static constexpr float deceleration = 0.95f;
};

class CaptureEffect : public Effect {
public:
    CaptureEffect(vec2 pos);
    void Update(double dt) override;
};

class HitEffect : public Effect {
public:
    HitEffect(vec2 pos);
    void Update(double dt) override;
};