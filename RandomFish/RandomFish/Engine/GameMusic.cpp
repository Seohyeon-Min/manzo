#include "GameMusic.h"
#include "Engine.h"

CS230::GameMusic::GameMusic()
{
	InitAudioDevice();
}

void CS230::GameMusic::Load(const std::filesystem::path& music_file,float given_volume)
{
	Unload();

	music = LoadMusicStream(music_file.string().c_str());
	volume = given_volume;
	SetMasterVolume(volume);
}

void CS230::GameMusic::Start()
{
	if(music.stream.buffer != nullptr) PlayMusicStream(music);
}

void CS230::GameMusic::Update()
{
	if (music.stream.buffer != nullptr) UpdateMusicStream(music);
}

void CS230::GameMusic::Pause()
{
	if (music.stream.buffer != nullptr) PauseMusicStream(music);
}

void CS230::GameMusic::Resume()
{
	if (music.stream.buffer != nullptr) ResumeMusicStream(music);
}

void CS230::GameMusic::Stop()
{
	if (music.stream.buffer != nullptr) StopMusicStream(music);
}

void CS230::GameMusic::SetVolume(float given_volume)
{
	volume = given_volume;
	SetMasterVolume(volume);
}

void CS230::GameMusic::Unload()
{
	UnloadMusicStream(music);
}

bool CS230::GameMusic::IsMusicPlaying()const
{
	return IsMusicStreamPlaying(music);
}

float CS230::GameMusic::GetVolume()const
{
	return volume;
}
