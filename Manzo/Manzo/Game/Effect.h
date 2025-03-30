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
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    ~CaptureEffect();
};

class GetFishEffect : public Effect {
public:
    GetFishEffect(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    float speed = 400.f;
    const float deceleration = 0.95f;
};

class HitEffect : public Effect {
public:
    HitEffect(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
};

class MonsterHitEffect : public Effect {
public:
    MonsterHitEffect(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
};

class BlackOutEffect : public Effect {
public:
    BlackOutEffect();
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    void SetAlpha(const GLShader* shader) override;
    RealTimeTimer* timer;
    double time = 1.0;
};