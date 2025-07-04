#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "Particles.h"
#include "Ship.h"
#include "../Engine/Sprite.h"
#include "JellyEnemy.h"

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
    double time = 1.3;
};

class BossBlackCircle : public Effect {
public:
    BossBlackCircle(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
};

class BossBlackCircle2 : public Effect {
public:
    BossBlackCircle2(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    void SetFadeinUni(const GLShader* shader);
    float spawn_t;
};

class BlackTransition : public Effect {
public:
    BlackTransition();
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
    void Quit() { stop = true; }
private:
    void SetUni(const GLShader* shader);
    float spawn_t;
    bool stop = false;
};

class CirclePattern : public Effect {
public:
    CirclePattern(float radius, float life = 4.f);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    void SetUni(const GLShader* shader);
    float spawn_t;
    float radius;
};

class Flash : public Effect {
public:
    Flash(float time = 1.f);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    void SetUni(const GLShader* shader);
    float life = 0;
    float max_life = 2.4f;
};

class Digipen : public Effect {
public:
    Digipen();
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
};

class Jellyfish : public Effect {
public:
    Jellyfish(JellyEnemy* jelly);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    JellyEnemy* jelly;
};

class JellyBullet : public Effect {
public:
    JellyBullet(vec2 pos);
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::DrawLast) override;
private:
    JellyEnemy* jelly;
};