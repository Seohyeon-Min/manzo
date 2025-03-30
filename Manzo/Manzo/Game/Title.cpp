#include "Title.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Particle.h"
#include "Particles.h"
#include "Background.h"
#include "Ship.h"
#include "Mouse.h"
#include "States.h"

Title::Title()
{
}

void Title::Load()
{
	Engine::GetShaderManager().LoadShader("under_water_god_ray", "assets/shaders/post_default.vert", "assets/shaders/underwater_god_ray.frag");
	Engine::GetShaderManager().LoadShader("post_default", "assets/shaders/post_default.vert", "assets/shaders/post_default.frag");
	Engine::GetShaderManager().LoadShader("post_bloom", "assets/shaders/post_default.vert", "assets/shaders/post_bloom.frag");
	Engine::GetShaderManager().LoadShader("post_underwater_distortion", "assets/shaders/post_default.vert", "assets/shaders/post_underwater_distortion.frag");
	Engine::GetShaderManager().LoadShader("title_ripple", "assets/shaders/post_default.vert", "assets/shaders/title_water_ripple.frag");
	Engine::GetShaderManager().LoadShader("title_gradation", "assets/shaders/post_default.vert", "assets/shaders/title_color_gradation.frag");
	Engine::GetShaderManager().LoadShader("image_distortion", "assets/shaders/default.vert", "assets/shaders/image_distortion.frag");
	Engine::GetShaderManager().LoadShader("post_water_wave", "assets/shaders/post_default.vert", "assets/shaders/post_water_wave.frag");


	AddGSComponent(new GameObjectManager());

	// background
	//background = new Background();
	//AddGSComponent(background);
	//background->Add("assets/images/background/Title.png", 1.f);

	// camera
	AddGSComponent(new Cam());
	GetGSComponent<Cam>()->SetPosition({ 0, 0 });

	// audio
	Engine::GetAudioManager().LoadMusic("assets/audios/100BPM.mp3", "title_bgm", false);

	// mouse
	GetGSComponent<GameObjectManager>()->Add(new Mouse);

	// Text
	GetGSComponent<GameObjectManager>()->Add(new TitleText({}));
}

void Title::Update(double dt)
{
	//audio play
	if (!playing)
	{
		Engine::GetAudioManager().PlayMusics("title_bgm");
		playing = true;
	}

	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<Cam>()->Update(dt, {}, false);

	// Move to next scean
	if (Engine::GetInput().MouseButtonJustReleased((SDL_BUTTON_LEFT))) {
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}
}

void Title::Draw()
{
	//GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();

	Engine::GetFontManager().PrintText(FontType::Thin, "CLICK TO START", { -72.f,-140.f }, 0.07f, { 1.f,1.f,1.f }, 0.7f);
}

void Title::FixedUpdate(double dt)
{
	if (GetGSComponent<GameObjectManager>()) {
		GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
	}
}

void Title::Unload()
{
	GetGSComponent<GameObjectManager>()->Unload();
	Engine::GetAudioManager().StopAllChannels();
	//GetGSComponent<Background>()->Unload();
	playing = false;
}

TitleText::TitleText(vec2 start_position) : GameObject({0.f, 50.f})
{
	AddGOComponent(new Sprite("assets/images/title_text.spt", this));
	SetScale({0.7f,0.7f});
}

void TitleText::Update(double dt)
{
	GameObject::Update(dt);
}

void TitleText::Draw(DrawLayer drawlayer)
{
	DrawCall draw_call = {
	GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
	&GetMatrix(),                          // Transformation matrix
	Engine::GetShaderManager().GetShader("image_distortion")
	};

	draw_call.settings.do_blending = true;
	draw_call.SetUniforms = [this](const GLShader* shader) { SetUniform(shader); };
	GameObject::Draw(draw_call);
}

void TitleText::SetUniform(const GLShader* shader) {
	double currentTime = Engine::GetAudioManager().GetCurrentMusicTime("title_bgm");
	shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
	shader->SendUniform("iTime", float(currentTime));
}