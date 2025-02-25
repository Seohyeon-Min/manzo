#include "Effect.h"
#include "../Engine/Sprite.h"

Effect::Effect(vec2 position, double effect_time)
    : GameObject(position), effect_time(effect_time) {
    effect_timer = new Timer(effect_time);
    AddGOComponent(effect_timer);
    SetPosition(position);
    SetCameraFixed(true);
}

Effect::~Effect() {
    ClearGOComponents();
}

void Effect::Update(double dt) {
    GameObject::Update(dt);
    if (effect_timer->Remaining() <= 0) {
        Destroy();
    }
}

void Effect::Draw(DrawLayer drawlayer) {

    Sprite* sprite = GetGOComponent<Sprite>();

    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("change_alpha") // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetAlpha(shader); };
    draw_call.sorting_layer = drawlayer;

    GameObject::Draw(draw_call);
}

void Effect::SetAlpha(const GLShader* shader) {
    float alpha = float(effect_timer->Remaining() / effect_time);
    shader->SendUniform("uAlpha", alpha);
}


DashEffect::DashEffect()
    : Effect({}, 0.5) {
    AddGOComponent(new Sprite("assets/images/dash_effect.spt", this));
    ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 dir = ship->GetVelocity().Normalize();
    float angle_radians = std::atan2(dir.y, dir.x);
    force = -dir * speed;
    SetScale({-1,1});
    SetPosition(ship->GetPosition());
    SetRotation(angle_radians);

    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BubblePop>>()
        ->Emit(8, GetPosition(), force * 0.4f, dir * -100.f, 1.5);
}

void DashEffect::Update(double dt) {
    Effect::Update(dt);
    SetVelocity(force);
    float base_dt = 1.0f / 240.f;
    force *= (float)std::pow(deceleration, dt / base_dt);
    if (effect_timer->Remaining() == 0) Destroy();
}

void DashEffect::Draw(DrawLayer drawlayer) {

    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("change_alpha") // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetAlpha(shader); };
    draw_call.sorting_layer = drawlayer;
    GameObject::Draw(draw_call);
}


CaptureEffect::CaptureEffect(vec2 pos)
    : Effect(pos, 0.5) {
    SetScale({ 0.6f, 0.6f });
    AddGOComponent(new Sprite("assets/images/Capture_effect.spt", this));
    GetGOComponent<Sprite>()->PlayAnimation(0);
}

void CaptureEffect::Update(double dt) {
    Effect::Update(dt);
    if (GetGOComponent<Sprite>()->GetCurrentFrame() >= 8) {
        Destroy();
    }
}

void CaptureEffect::Draw(DrawLayer drawlayer)
{
    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("change_alpha") // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetAlpha(shader); };
    draw_call.sorting_layer = DrawLayer::DrawPlayerTop;
    GameObject::Draw(draw_call);
}

CaptureEffect::~CaptureEffect()
{
    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::CaptureEffect>>()
        ->EmitRound(2, GetPosition(), 100.f, 30.f);
}

HitEffect::HitEffect(vec2 pos)
    : Effect(pos, 0.5) {
    SetScale({ 0.6f, 0.6f });
    AddGOComponent(new Sprite("assets/images/Hit_effect.spt", this));
    GetGOComponent<Sprite>()->PlayAnimation(0);

    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::HitPraticle>>()
        ->EmitRound(20, GetPosition(), 1800.f, 600.f);
    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::HitPraticle2>>()
        ->EmitRound(10, GetPosition(), 1500.f, 600.f);
}

void HitEffect::Update(double dt) {
    Effect::Update(dt);
    if (GetGOComponent<Sprite>()->AnimationEnded()) {
        Destroy();
    }
}

void HitEffect::Draw(DrawLayer drawlayer)
{
    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("change_alpha") // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetAlpha(shader); };
    draw_call.sorting_layer = DrawLayer::DrawLast;
    GameObject::Draw(draw_call);
}
