#pragma once

#include "../FMOD/fmod_studio.hpp"
#include "../FMOD/fmod.hpp"
#include "vec3.h"

#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

struct Implementation {
	Implementation();
	~Implementation();

	void Update(double slow);

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	std::string mChannelId;

	//BGM
	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<std::string, FMOD::Channel*> ChannelMap;

	//SFX
	std::map<std::string, FMOD::Sound*> mEffects;
	std::map<std::string, FMOD::Channel*> mEffectChannels;

	SoundMap mSounds;
	ChannelMap mChannels;
};

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void Update();
	static int ErrorCheck(FMOD_RESULT result);
	void SetSlowDownFactor(double slow) { slow_down = slow; }

	// music
	void LoadMusic(const std::string& filePath, const std::string& alias, bool b3d = false, bool bLooping = true, bool bStream = false);
	void UnLoadMusic(const std::string& alias);
	FMOD::Sound* GetMusic(const std::string& alias);
	float GetCurrentMusicTime(const std::string& alias);
	float GetCurrentPlayingMusicTime();
	float GetMusicLength(const std::string& alias);
	std::string GetID(const std::string& alias);

	std::string PlayMusics(const std::string& alias, const vec3& vPos = vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void StopChannel(const std::string& alias);
	void StopAllChannels();
	void RestartPlayMusic(const std::string& alias);
	void StopPlayingMusic(const std::string& alias);
	bool IsPlayingMusic(const std::string& alias) const;

	bool IsAnyMusicPlaying() const;

	bool IsMusicFinished(const std::string& alias);

	void Set3dListenerAndOrientation(const vec3& vPosition, const vec3& vLook, const vec3& vUp);
	void SetChannel3dPosition(const std::string& alias, const vec3& vPosition);
	void SetMode(const std::string& alias, bool spatial_on);

	void Set3DMode(FMOD_MODE mode);
	
	void SetChannelVolume(const std::string& alias, float fVolumedB);
	void SetMute(const std::string& alias, bool mute); //if true, mute
	const bool GetMute() {
		return isMute;
	}

	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const vec3& vPosition);

	// SFX
	void LoadSound(const std::string& filePath, const std::string& alias, bool b3d = false, bool bLooping = false, bool bStream = false);
	std::string PlaySound(const std::string& alias, const vec3& vPos = vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void StopSound(const std::string& alias);

private:
	bool isMute = false;
	double slow_down = 1;
};
