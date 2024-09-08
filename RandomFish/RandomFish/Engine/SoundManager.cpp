#include "SoundManager.h"

namespace CS230
{
    void SoundManager::AddSound(const std::string& id, const std::filesystem::path& sound_file, float volume)
    {
        sounds.emplace(id, GameSound(sound_file, volume));
    }

    void SoundManager::Play(const std::string& id)
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            it->second.Start();
        }
    }

    void SoundManager::Pause(const std::string& id)
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            it->second.Pause();
        }
    }

    void SoundManager::Resume(const std::string& id)
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            it->second.Resume();
        }
    }

    void SoundManager::Stop(const std::string& id)
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            it->second.Stop();
        }
    }

    void SoundManager::SetVolume(const std::string& id, float volume)
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            it->second.SetVolume(volume);
        }
    }

    void SoundManager::PlayAll()
    {
        for (auto& sound : sounds)
        {
            sound.second.Start();
        }
    }

    void SoundManager::PauseAll()
    {
        for (auto& sound : sounds)
        {
            sound.second.Pause();
        }
    }

    void SoundManager::ResumeAll()
    {
        for (auto& sound : sounds)
        {
            sound.second.Resume();
        }
    }

    void SoundManager::StopAll()
    {
        for (auto& sound : sounds)
        {
            sound.second.Stop();
        }
    }

    void SoundManager::SetVolumeAll(float volume)
    {
        for (auto& sound : sounds)
        {
            sound.second.SetVolume(volume);
        }
    }

    bool SoundManager::SoundPlaying(const std::string& id) const
    {
        auto it = sounds.find(id);
        if (it != sounds.end())
        {
            return it->second.Playing();
        }
        return false;
    }

    void SoundManager::Unload(const std::string& id)
    {
        for (auto& sound : sounds)
        {
            sound.second.Unload();
        }
    }

}
