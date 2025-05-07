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
	background->Add("assets/images/background/house.png", 0.25f);

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

	default_radius = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->GetMinRadius();
	variable_radius = default_radius;

}

void Tutorial::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<Cam>()->Update(dt, {}, false);
	beat_system->Update(dt);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
		//if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}

	if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT))
	{
		variable_radius = default_radius;
		variable_radius += static_cast<float>(variable_radius * beat_system->GetLastCali());
		//Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Pump>()->SetMinRadius(variable_radius);
	}
}

void Tutorial::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();

	CircleDrawCall draw_call = {
		variable_radius,
		ship_ptr->GetPosition()
	};
	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawUI;
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