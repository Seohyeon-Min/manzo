#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "Particles.h"
#include "Ship.h"
#include "../Engine/Sprite.h"

class DashEffect : public GameObject {
public:
    DashEffect() :
        GameObject({}), effect_timer(new Timer(effect_time)){
        AddGOComponent(new Sprite("assets/images/dash_effect.spt", this));
        ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
        vec2 dir = ship->GetVelocity().Normalize();
        float angle_radians = std::atan2(dir.y, dir.x);
        force = -dir * speed;
        SetPosition(ship->GetPosition());
        SetRotation(angle_radians);
        AddGOComponent(effect_timer);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BubblePop>>()->Emit(8, GetPosition(), force * 0.4f , dir * -100.f, 1.5);
    }
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "DashEffect Particle"; }
    void Update(double dt) override {
        GameObject::Update(dt);
        SetVelocity(force);
        float base_dt = 1.0f / 240.f;
        float adjusted_deceleration = (float)pow(deceleration, dt / base_dt);
        force *= adjusted_deceleration;
        if (effect_timer->Remaining() == 0) Destroy();
    }
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override
    {
        DrawSettings settings;
        settings.do_blending = true;
        Sprite* sprite = GetGOComponent<Sprite>();

        DrawCall draw_call = {
            sprite,
            &GetMatrix(),
            Engine::GetShaderManager().GetShader("change_alpha"),
            [this](const GLShader* shader) {
                SetAlpha(shader);
            },
            settings
        };

        GameObject::Draw(draw_call);
    }

    void SetAlpha(const GLShader* shader)
    {
        float alpha = float(effect_timer->Remaining() / effect_time);
        shader->SendUniform("uAlpha", alpha);
    }

private:
    Ship* ship;
    const double effect_time = 0.5;
    static constexpr float deceleration = 0.95f;
    float speed = 1000;
    vec2 force = {};
    Timer* effect_timer;
};


class CaptureEffect : public GameObject {
public:
    CaptureEffect(vec2 pos) :
        GameObject(pos) {
        SetScale({ 0.6f,0.6f });
        AddGOComponent(new Sprite("assets/images/Capture_effect.spt", this));
        GetGOComponent<Sprite>()->PlayAnimation(0);
    }
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "CaptureEffect"; }
    void Update(double dt) override {
        GameObject::Update(dt);
        if (GetGOComponent<Sprite>()->AnimationEnded()) {
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::CaptureEffect>>()->EmitRound(8, GetPosition(), 100.f, 30.f);
            Destroy();
        }
    }
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override
    {
        DrawSettings settings;
        settings.do_blending = true;
        Sprite* sprite = GetGOComponent<Sprite>();

        DrawCall draw_call = {
            sprite,
            &GetMatrix(),
            Engine::GetShaderManager().GetDefaultShader(),
            nullptr,
            settings
        };

        GameObject::Draw(draw_call);
    }
private:
    CaptureEffect() :
        GameObject({}) {};
};

class HitEffect : public GameObject {
public:
    HitEffect(vec2 pos) :
        GameObject(pos) {
        SetScale({ 0.6f,0.6f });
        AddGOComponent(new Sprite("assets/images/Hit_effect.spt", this));
        GetGOComponent<Sprite>()->PlayAnimation(0);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::CaptureEffect>>()->EmitRound(8, GetPosition(), 100.f, 30.f);
    }
    GameObjectTypes Type() override { return GameObjectTypes::Ship; }
    std::string TypeName() override { return "HitEffect"; }
    void Update(double dt) override {
        GameObject::Update(dt);
        if (GetGOComponent<Sprite>()->AnimationEnded()) {
            Destroy();
        }
    }
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override
    {
        DrawSettings settings;
        settings.do_blending = true;
        Sprite* sprite = GetGOComponent<Sprite>();

        DrawCall draw_call = {
            sprite,
            &GetMatrix(),
            Engine::GetShaderManager().GetDefaultShader(),
            nullptr,
            settings
        };

        GameObject::Draw(draw_call, DrawLayer::DrawUI);
    }
private:
    HitEffect() :
        GameObject({}) {};
};