#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include "Component.h"
#include <iostream>

class AudioManager : public CS230::Component {
public:
    AudioManager();
    ~AudioManager();

    bool Init();
    void CleanUp();

    Mix_Chunk* LoadSound(const std::string& filePath, const std::string& name);
    Mix_Music* LoadMusic(const std::string& filePath, const std::string& name);

    void PlaySound(Mix_Chunk* sound, int loops = 0);
    void PlayMusic(Mix_Music* music, int loops = -1);
    void StopMusic();
    //Mix_Music* GetMusic(const std::string& name);
    double GetCurrentMusicTime();

private:
    std::map<std::string, Mix_Chunk*> soundEffects;
    std::map<std::string, Mix_Music*> musicTracks;
};
