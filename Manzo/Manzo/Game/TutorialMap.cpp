#include "TutorialMap.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Particle.h"
#include "Particles.h"
#include "Background.h"
#include "Ship.h"
#include "Mouse.h"
#include "States.h"

Tutorial::Tutorial()
{
}

void Tutorial::Load()
{
	AddGSComponent(new GameObjectManager());



	// background
	background = new Background();
	AddGSComponent(background);
	background->Add("assets/images/background/cali_back.png", 0.25f);

	// camera
	AddGSComponent(new Cam());
	GetGSComponent<Cam>()->SetPosition({ 0, 0 });

	// audio
	beat_system = &Engine::GetBeat();
	Engine::GetAudioManager().LoadMusic("assets/audios/100BPM.mp3", "tutorial_bgm", false);
	beat_system->LoadMusicToSync("tutorial_bgm");
	beat_system->ResetCalibration();

	// ship
	ship_ptr = new Ship({ 0, 0 });
	GetGSComponent<GameObjectManager>()->Add(ship_ptr);

	AddGSComponent(new ParticleManager<Particles::FuelBubble>());
	AddGSComponent(new ParticleManager<Particles::BubblePop>());
	AddGSComponent(new ParticleManager<Particles::HitPraticle>());
	AddGSComponent(new ParticleManager<Particles::HitPraticle2>());
	AddGSComponent(new ParticleManager<Particles::CaptureEffect>());

	// mouse
	GetGSComponent<GameObjectManager>()->Add(new Mouse);

	default_min_radius = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->GetMinRadius() * 4.5f;
	default_max_radius = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->GetMaxRadius() * 4.5f;
	variable_min_radius = default_min_radius;
	//variable_min_radius = defalut_max_radius;
	ship_ptr->SetScale({ 3.5f, 3.5f });
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->SetMinRadius(default_min_radius);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->SetMaxRadius(default_max_radius);
}

void Tutorial::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<Cam>()->Update(dt, {}, false);
	beat_system->Update(dt);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Enter)) {
		if (!Engine::GetGameStateManager().FromOption())
		{
			Engine::GetGameStateManager().ClearNextGameState();
			Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode3));
		}
		else
		{
			Engine::GetGameStateManager().ClearNextGameState();
			Engine::GetGameStateManager().LoadPreviousGameState();
		}
	}

	if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT))
	{
		variable_min_radius = default_min_radius;
		variable_min_radius += static_cast<float>(4 * variable_min_radius * beat_system->GetLastCali());
		//Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->SetMinRadius(variable_min_radius);
	}
}

void Tutorial::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();

	CircleDrawCall draw_call = {
		variable_min_radius,
		ship_ptr->GetPosition(),
		{255,0,0}
	};
	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawUI;
	draw_call.order_in_layer = 9;
	Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));
}

void Tutorial::FixedUpdate(double dt)
{
	if (GetGSComponent<GameObjectManager>()) {
		GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
	}
}

void Tutorial::Unload()
{

	delete ship_ptr;
	ship_ptr = nullptr;
	playing = false;
	//beat_system = nullptr;
	beat_system->CalculateCali();
	beat_system->CleartoOriginal();

 	GetGSComponent<GameObjectManager>()->Unload();
	GetGSComponent<Background>()->Unload();
	Engine::GetRender().ClearDrawCalls();
	ClearGSComponents();
	Engine::GetAudioManager().StopAllChannels();
}