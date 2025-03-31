#include "TutorialMap.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Particle.h"
#include "Particles.h"
#include "BeatSystem.h"
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

	beat_system = new Beat();
	AddGSComponent(beat_system);

	// background
	background = new Background();
	AddGSComponent(background);
	background->Add("assets/images/background/house.png", 0.25f);

	// camera
	AddGSComponent(new Cam());
	GetGSComponent<Cam>()->SetPosition({ 0, 0 });

	// audio
	Engine::GetAudioManager().LoadMusic("assets/audios/100BPM.mp3", "tutorial_bgm", false);
	beat_system->LoadMusicToSync("tutorial_bgm");

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

}

void Tutorial::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<Cam>()->Update(dt, {}, false);

	if (Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
		//if (ship_ptr->IsShipUnder() && Engine::GetInput().KeyJustPressed(Input::Keys::Q)) {
		Engine::GetGameStateManager().ClearNextGameState();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode2));
	}

	//audio play
	if (!playing)
	{
		Engine::GetAudioManager().PlayMusics("tutorial_bgm");
		playing = true;
	}
}

void Tutorial::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();
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
}