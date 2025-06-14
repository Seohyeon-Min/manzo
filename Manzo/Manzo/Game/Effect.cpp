#include "Effect.h"
#include "../Engine/Sprite.h"
#include "States.h"

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
    if (effect_time <= -1.f) return;
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
    AddGOComponent(new Sprite("assets/images/effect/dash_effect.spt", this));
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
    AddGOComponent(new Sprite("assets/images/effect/Capture_effect.spt", this));
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

GetFishEffect::GetFishEffect(vec2 pos)
    : Effect(pos, 0.5) {
    //SetScale({ 0.6f, 0.6f });
    AddGOComponent(new Sprite("assets/images/effect/Get_fish_effect.spt", this));
}

void GetFishEffect::Update(double dt)
{
    Effect::Update(dt);
    SetVelocity({ GetVelocity().x, speed });
    float base_dt = 1.0f / 240.f;
    speed *= (float)std::pow(deceleration, dt / base_dt);
}

void GetFishEffect::Draw(DrawLayer drawlayer)
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

HitEffect::HitEffect(vec2 pos)
    : Effect(pos, 0.5) {
    SetScale({ 0.6f, 0.6f });
    AddGOComponent(new Sprite("assets/images/effect/hit_effect.spt", this));
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

MonsterHitEffect::MonsterHitEffect(vec2 pos)
: Effect(pos, 0.5) {
        SetScale({ 0.6f, 0.6f });
        AddGOComponent(new Sprite("assets/images/effect/monster_hit_effect.spt", this));
        GetGOComponent<Sprite>()->PlayAnimation(0);

        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::HitPraticle>>()
            ->EmitRound(20, GetPosition(), 1800.f, 600.f);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::HitPraticle2>>()
            ->EmitRound(10, GetPosition(), 1500.f, 600.f);
}

void MonsterHitEffect::Update(double dt)
{
    Effect::Update(dt);
    if (GetGOComponent<Sprite>()->AnimationEnded()) {
        Destroy();
    }
}

void MonsterHitEffect::Draw(DrawLayer drawlayer)
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

BlackOutEffect::BlackOutEffect() : Effect({}, 0.5)
{
    AddGOComponent(new Sprite("assets/images/effect/full_quad.spt", this));
    timer = new RealTimeTimer(time);
    AddGOComponent(timer);
    timer->Set(time);
    timer->Start();
}

void BlackOutEffect::Update(double dt)
{
    //GameObject::Update(dt);
        // Move to next scean

    if (timer->IsFinished()) {
        if (Engine::GetInput().MouseButtonJustReleased((SDL_BUTTON_LEFT))) {
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
        }
    }
}

void BlackOutEffect::Draw(DrawLayer drawlayer)
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
    draw_call.settings.is_camera_fixed = true;
    GameObject::Draw(draw_call);
}

void BlackOutEffect::SetAlpha(const GLShader* shader)
{
    float alpha = float(1.0 - timer->Remaining());
    //std::cout << timer->Remaining() << std::endl;
    //if (alpha >= 0.3f) alpha = 0.3f;
    shader->SendUniform("uAlpha", alpha);
}

BossBlackCircle::BossBlackCircle(vec2 pos) : Effect(pos, -1.f)
{
    AddGOComponent(new Sprite("assets/images/effect/boss_black_circle.spt", this));
}

void BossBlackCircle::Update(double dt)
{
    Effect::Update(dt);
    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BossBlackCircleParticle>>()
        ->EmitRound(1, GetPosition(), 20.f, 10.f, 115.f);
}

void BossBlackCircle::Draw(DrawLayer drawlayer)
{
    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader()
    };

    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = drawlayer;
    GameObject::Draw(draw_call);
}

BossBlackCircle2::BossBlackCircle2(vec2 pos) : Effect(pos, -1.f)
{
    AddGOComponent(new Sprite("assets/images/effect/boss_black_circle.spt", this));
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }
    spawn_t = time;
}

void BossBlackCircle2::Update(double dt)
{
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }
    if (time >= 15.f + spawn_t) {
        Destroy();
    }
}


void BossBlackCircle2::SetFadeinUni(const GLShader* shader)
{
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }
    shader->SendUniform("uTime", time);
    shader->SendUniform("uStartDelay", spawn_t + 9.f);
    shader->SendUniform("uFadeDuration", 3.f);

    //std::cout << time << " - time : " << spawn_t<< "\n";
}

void BossBlackCircle2::Draw(DrawLayer drawlayer)
{
    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("fade_out")
    };

    draw_call.settings.do_blending = true;
    draw_call.sorting_layer = DrawLayer::DrawLast;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetFadeinUni(shader); };
    GameObject::Draw(draw_call);
}

BlackTransition::BlackTransition() : Effect({}, -1.f)
{
    AddGOComponent(new Sprite("assets/images/effect/full_quad.spt", this));
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }
    spawn_t = time;
}

void BlackTransition::Update(double dt)
{
    if (stop) Destroy();
}

void BlackTransition::Draw(DrawLayer drawlayer)
{
    Sprite* sprite = GetGOComponent<Sprite>();
    DrawCall draw_call = {
        sprite,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("ink_transition")
    };

    draw_call.settings.do_blending = true;
    draw_call.settings.is_camera_fixed = true;
    draw_call.sorting_layer = DrawLayer::DrawFirst;
    draw_call.order_in_layer = 1;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetUni(shader); };
    GameObject::Draw(draw_call);
}

void BlackTransition::SetUni(const GLShader* shader)
{
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }

    shader->SendUniform("uTime", time);
    shader->SendUniform("uStartTime", spawn_t);
    shader->SendUniform("uDelay", 18.7f);
    shader->SendUniform("uResolution", Engine::window_width, Engine::window_height);
}
