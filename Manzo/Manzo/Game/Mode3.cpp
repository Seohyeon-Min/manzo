 
#include "Mode3.h"
#include "../Engine/Engine.h"
#include "../Engine/Timer.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/AudioManager.h"
#include "../Engine/Particle.h"
#include "../Engine/MapManager.h"
#include "../Engine/Rapidjson.h"
#include "../Engine/UI.h"

#include "DialogBox.h"
#include "Particles.h"
#include "Mouse.h"
#include "States.h"
#include "Mode1.h"
#include "Background.h"
#include "Ship.h"
#include "Fish.h"
#include "Skill.h"
#include "Boss.h"
#include "Monster.h"

#include <utility>
#include <iostream>

Mode3::Mode3() 
{
}

void Mode3::Load() {
	// audio
	Engine::GetAudioManager().LoadMusic("assets/audios/tutorial.mp3", "tutorial_bgm", false);

	// component
	AddGSComponent(new GameObjectManager());
	beat_system = &Engine::GetBeat();
	beat_system->LoadMusicToSync("tutorial_bgm");

	//// camera
    AddGSComponent(new Cam());
    GetGSComponent<Cam>()->SetPosition({ 0, 0 });

    // Particle
    AddGSComponent(new ParticleManager<Particles::Plankton>());
    AddGSComponent(new ParticleManager<Particles::FuelBubble>());
    AddGSComponent(new ParticleManager<Particles::BubblePop>());
    AddGSComponent(new ParticleManager<Particles::HitPraticle>());
    AddGSComponent(new ParticleManager<Particles::HitPraticle2>());
    AddGSComponent(new ParticleManager<Particles::CaptureEffect>());
    AddGSComponent(new ParticleManager<Particles::BulletParticle>());

    // background
    background = new Background();
    AddGSComponent(background);
    background->Add("assets/images/background/tutorial.png", 0.25f);

	//// ship
	ship_ptr = new Ship({0,0});
	GetGSComponent<GameObjectManager>()->Add(ship_ptr);

    //Dialog
    dialog_ptr = new Dialog({ 0,0 });
    GetGSComponent<GameObjectManager>()->Add(dialog_ptr);

    // Mouse
    GetGSComponent<GameObjectManager>()->Add(new Mouse);


}

void Mode3::Update(double dt) {
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt);
    GetGSComponent<Cam>()->Update(dt, {}, false);
    beat_system->Update(dt);
    phaseTimer += dt;
    std::cout << "Suc: " << ship_ptr->GetDashSuccess() << std::endl;
    int count = 0;
    switch (currentPhase) {
    case TutorialPhase::Init:

        if (!textDisplay ) {

            dialog_ptr->LoadDialogGroup("tutorial-1", 0.05);
            textDisplay = true;
            phaseTimer = 0;
        }
        if (Engine::GetInput().KeyJustPressed(Input::Keys::Space)) {
            dialog_ptr->NextLine();
            if (dialog_ptr->IsFinished()) {
                currentPhase = TutorialPhase::Dash;
            }
        }
        break;
    case TutorialPhase::Dash:
        std::cout << "yes" << std::endl;
        break;

    case TutorialPhase::Done:
        //Engine::GetFontManager().PrintText(FontType::Thin, FontAlignment::CENTER, "", { 0,0 }, 0.05f, { 1.0f, 1.0f, 1.0f }, 1.f);
        break;
    }
}


void Mode3::FixedUpdate(double dt)
{
    if (GetGSComponent<GameObjectManager>()) {
        GetGSComponent<GameObjectManager>()->FixedUpdateAll(dt);
    }
}

void Mode3::Draw() {
    if (GetGSComponent<Background>() && GetGSComponent<Cam>()) {
        GetGSComponent<Background>()->Draw(*GetGSComponent<Cam>());
    }
    GetGSComponent<GameObjectManager>()->DrawAll();
    dialog_ptr->Draw();


}


void Mode3::Unload() {
    Engine::GetAudioManager().StopAllChannels();
    GetGSComponent<GameObjectManager>()->Unload();
    GetGSComponent<Background>()->Unload();
    Engine::GetIconManager().Unload();
    ClearGSComponents();
    dialog_ptr->Unload();
    background = nullptr;

}
