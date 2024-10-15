#include "AudioManager.h"

AudioManager::AudioManager() {
    Init();
}

AudioManager::~AudioManager() {
    CleanUp();
}

bool AudioManager::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "Mix_Init error: " << Mix_GetError() << std::endl;
        //SDL_Quit();
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;
        Mix_Quit();
        //SDL_Quit();
        return false;
    }
    return true;
}

void AudioManager::CleanUp() {
    for (auto& sound : soundEffects) {
        Mix_FreeChunk(sound.second);
    }
    for (auto& music : musicTracks) {
        Mix_FreeMusic(music.second);
    }
    Mix_CloseAudio();
    //Mix_Quit();
    //SDL_Quit();
}

Mix_Chunk* AudioManager::LoadSound(const std::string& filePath, const std::string& name) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (sound) {
        soundEffects[name] = sound;
    }
    return sound;
}

Mix_Music* AudioManager::LoadMusic(const std::string& filePath, const std::string& name) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (music) {
        musicTracks[name] = music;
    }
    return music;
}

void AudioManager::PlaySound(Mix_Chunk* sound, int loops) {
    Mix_PlayChannel(-1, sound, loops);
}

void AudioManager::PlayMusic(Mix_Music* music, int loops) {
    Mix_PlayMusic(music, loops);
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
}

//Mix_Music* AudioManager::GetMusic(const std::string& name) {
//    auto it = musicTracks.find(name);
//    if (it != musicTracks.end()) {
//        return it->second;
//    }
//    else {
//        std::cerr << "Error: Music track with name '" << name << "' not found." << std::endl;
//        return nullptr;
//    }
//}

double AudioManager::GetCurrentMusicTime() {
    if (Mix_PlayingMusic()) {
        return Mix_GetMusicPosition(nullptr);  // 현재 음악의 재생 위치를 반환합니다.
    }
    else {
        std::cerr << "No music is currently playing." << std::endl;
        return 0;
    }
}