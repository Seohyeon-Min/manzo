#include "AudioManager.h"

#include "../FMOD/fmod.hpp"
#include "../FMOD/fmod_errors.h"
#include "../FMOD/fmod_studio.hpp"

Implementation::Implementation() {
	mpStudioSystem = NULL;
	mpSystem = NULL;
	AudioManager::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));

	AudioManager::ErrorCheck(mpStudioSystem->initialize(
		32,
		FMOD_STUDIO_INIT_LIVEUPDATE,
		FMOD_INIT_NORMAL,
		nullptr 
	));

	AudioManager::ErrorCheck(mpStudioSystem->getCoreSystem(&mpSystem));

	int driverCount = 0;
	AudioManager::ErrorCheck(mpSystem->getNumDrivers(&driverCount));
	std::cout << "Available audio drivers: " << driverCount << std::endl;
}

Implementation::~Implementation() {
	AudioManager::ErrorCheck(mpStudioSystem->unloadAll());
	AudioManager::ErrorCheck(mpStudioSystem->release());
}

void Implementation::Update() {
	std::vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	AudioManager::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

AudioManager::AudioManager()
{
	sgpImplementation = new Implementation;
}

AudioManager::~AudioManager()
{
	delete sgpImplementation;
}

void AudioManager::Update() {
	sgpImplementation->Update();
}

void AudioManager::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	FMOD::Sound* pSound = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
	}
}

void AudioManager::UnLoadSound(const std::string& strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;
	ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

int AudioManager::PlaySounds(const std::string& strSoundName, const vec3& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		LoadSound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

void AudioManager::StopChannel(int nChannelId)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt != sgpImplementation->mChannels.end()) {
		ErrorCheck(tFoundIt->second->stop());
		sgpImplementation->mChannels.erase(tFoundIt);
	}
}

void AudioManager::StopAllChannels()
{
	for (auto& channelPair : sgpImplementation->mChannels) {
		ErrorCheck(channelPair.second->stop());
	}
	sgpImplementation->mChannels.clear();
}

void AudioManager::Set3dListenerAndOrientation(const vec3& vPosition, const vec3& vLook, const vec3& vUp) {
	if (!sgpImplementation->mpSystem)
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	FMOD_VECTOR forward = VectorToFmod(vLook);
	FMOD_VECTOR up = VectorToFmod(vUp);

	ErrorCheck(sgpImplementation->mpSystem->set3DListenerAttributes(0, &position, nullptr, &forward, &up));
}

void AudioManager::SetChannel3dPosition(int nChannelId, const vec3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void AudioManager::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

bool AudioManager::IsPlaying(int nChannelId) const
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt != sgpImplementation->mChannels.end()) {
		bool bIsPlaying = false;
		ErrorCheck(tFoundIt->second->isPlaying(&bIsPlaying));
		return bIsPlaying;
	}
	return false;
}

FMOD_VECTOR AudioManager::VectorToFmod(const vec3& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int AudioManager::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		std::cout << "FMOD ERROR " << result << std::endl;
		return 1;
	}
	return 0;
}

float AudioManager::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioManager::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}