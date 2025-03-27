#include "Title.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Particle.h"
#include "Particles.h"
#include "Background.h"
#include "Ship.h"
#include "Mouse.h"

Title::Title()
{
}

void Title::Load()
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
	Engine::GetAudioManager().LoadMusic("assets/audios/100BPM.mp3", "tutorial_bgm", false);


	// mouse
	GetGSComponent<GameObjectManager>()->Add(new Mouse);

}

void Title::Update(double dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt);
	GetGSComponent<Cam>()->Update(dt, {}, false);

	//audio play
	if (!playing)
	{
		Engine::GetAudioManager().PlayMusics("tutorial_bgm");
		playing = true;
	}
}

void Title::Draw()
{
	GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
	GetGSComponent<GameObjectManager>()->DrawAll();
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
	GetGSComponent<Background>()->Unload();
	playing = false;
}