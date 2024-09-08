#pragma once

#include <unordered_map>
#include <filesystem>
#include "GameSound.h"

namespace CS230
{
    class SoundManager
    {
    private:
        std::unordered_map<std::string, GameSound> sounds;

    public:
        void AddSound(const std::string& id, const std::filesystem::path& sound_file, float volume = 1.0f);

        void Play(const std::string& id);

        void Pause(const std::string& id);

        void Resume(const std::string& id);

        void Stop(const std::string& id);

        void SetVolume(const std::string& id, float volume);

        void PlayAll();

        void PauseAll();

        void ResumeAll();

        void StopAll();

        void SetVolumeAll(float volume);

        bool SoundPlaying(const std::string& id) const;

        void Unload(const std::string& id);
    };
}
