#pragma once
#include "..\Engine\GameObject.h"
#include "GameObjectTypes.h"
#include "Ship.h"
#include "../Engine/Sprite.h"

class DashEffect : public CS230::GameObject {
public:
    DashEffect() :
        GameObject({}), effect_timer(new CS230::Timer(effect_time)){
        AddGOComponent(new CS230::Sprite("assets/images/dash_effect.spt", this));
        ship = Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->GetGOComponent<Ship>();
        vec2 dir = ship->GetVelocity().Normalize();
        float angle_radians = std::atan2(dir.y, dir.x);
        force = -dir * speed;
        SetPosition(ship->GetPosition());
        SetRotation(angle_radians);
        AddGOComponent(effect_timer);
        Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::BubblePop>>()->Emit(8, GetPosition(), force * 0.4f , dir * -100.f, 1.5);
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

        CS230::DrawCall draw_call = {
            GetGOComponent<CS230::Sprite>()->GetTexture(),
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
    CS230::Timer* effect_timer;
};