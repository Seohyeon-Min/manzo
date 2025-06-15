/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Splash.cpp
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "../Engine/Engine.h"
#include "States.h"
#include "Splash.h"
#include "Effect.h"
#include "../Engine/Camera.h"


Splash::Splash() {

}

void Splash::Load() {
    counter = 0;

    AddGSComponent(new GameObjectManager());

    // camera
    AddGSComponent(new Cam());
    GetGSComponent<Cam>()->SetPosition({ 0, 0 });

    // audio
    Engine::GetAudioManager().LoadMusic("assets/audios/title_beginning_short.mp3", "splash", false);

    Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Digipen());

}


void Splash::Update([[maybe_unused]] double dt) {
    static bool start = 0;
    if (Engine::GetInput().MouseButtonJustReleased((SDL_BUTTON_LEFT))) {
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new Flash(3.7f));
        start = 1;
    }
    if (start) {
        static bool playing = false;
        UpdateGSComponents(dt);
        GetGSComponent<GameObjectManager>()->UpdateAll(dt);
        GetGSComponent<Cam>()->Update(dt, {}, false);
        if (!playing)
        {
            Engine::GetAudioManager().PlayMusics("splash");
            playing = true;
        }
        // Move to next scean
        if (Engine::GetInput().MouseButtonJustReleased((SDL_BUTTON_RIGHT))) {
            Engine::GetAudioManager().StopPlayingMusic("splash");
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Title));
        }
        Engine::GetLogger().LogDebug(std::to_string(counter));
        if (counter >= 5) {
            Engine::GetGameStateManager().ClearNextGameState();
        }
        counter += dt;
    }

}

void Splash::FixedUpdate(double dt)
{
}

void Splash::Unload() {
    GetGSComponent<GameObjectManager>()->Unload();
    Engine::GetAudioManager().StopAllChannels();
    Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Title));
}


void Splash::Draw() {
    GetGSComponent<GameObjectManager>()->DrawAll();
    //Engine::GetWindow().Clear(UINT_MAX);
    //auto texture = TextureManager::GetInstance().GetTexture("texture_path");

    //texture->Draw(mat3({ (Engine::GetWindow().GetSize() - texture->GetSize()) / 2.0f }));
}
