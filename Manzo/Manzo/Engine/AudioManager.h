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

	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	int mnNextChannelId = 0;

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;
};

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	static void Update();
	static int ErrorCheck(FMOD_RESULT result);

	void LoadSound(const std::string& strSoundName, bool b3d = false, bool bLooping = true, bool bStream = false);
	void UnLoadSound(const std::string& strSoundName);

	FMOD::Sound* GetMusic(const std::string& strMusicName);
	float GetCurrentMusicTime(int nChannelId);
	int GetID(const std::string& soundName);

	int PlaySounds(const std::string& strSoundName, const vec3& vPos = vec3{ 0, 0, 0 }, float fVolumedB = 10.0f);
	void StopChannel(int nChannelId);
	void StopAllChannels();
	void RestartPlay(int nChannelId);
	void StopPlaying(int nChannelId);
	bool IsPlaying(int nChannelId) const;

	void Set3dListenerAndOrientation(const vec3& vPosition, const vec3& vLook, const vec3& vUp);
	void SetChannel3dPosition(int nChannelId, const vec3& vPosition);
	void SetMode(const std::string& strSoundName, bool spatial_on);
	
	void SetChannelVolume(int nChannelId, float fVolumedB);
	void SetMute(int nChannelId, bool mute); //if true, mute
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const vec3& vPosition);
};
