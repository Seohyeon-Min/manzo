#include "GameSound.h"
#include "Engine.h"

CS230::GameSound::GameSound(const std::filesystem::path& sound_file, float volume)
{
	Load(sound_file, volume);
}

CS230::GameSound::~GameSound()
{
	Unload();
}

void CS230::GameSound::Load(const std::filesystem::path& sound_file, float given_volume)
{
	Unload();

	sound = LoadSound(sound_file.string().c_str());

	if (sound.stream.buffer == nullptr)
	{
		Engine::GetLogger().LogError("Failed to load sound: " + sound_file.string());
		return;
	}
	volume = given_volume;
	SetMasterVolume(volume);
}

void CS230::GameSound::Start()
{
	if (sound.stream.buffer != nullptr) PlaySound(sound);
}

void CS230::GameSound::Pause()
{
	if (sound.stream.buffer != nullptr) PauseSound(sound);
}

void CS230::GameSound::Resume()
{
	if (sound.stream.buffer != nullptr) ResumeSound(sound);
}

void CS230::GameSound::Stop()
{
	if(sound.stream.buffer != nullptr) StopSound(sound);
}

void CS230::GameSound::SetVolume(float volume)
{
	this->volume = volume;
	SetSoundVolume(sound, volume);
}

void CS230::GameSound::Unload()
{
	if (sound.stream.buffer != nullptr)
	{
		UnloadSound(sound);
		delete sound.stream.buffer;
		sound.stream.buffer = nullptr;
	}
}

bool CS230::GameSound::Playing() const
{
	return IsSoundPlaying(sound);
}
