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


	FMOD::ChannelGroup* masterGroup = nullptr;
	AudioManager::ErrorCheck(mpSystem->getMasterChannelGroup(&masterGroup));
}

Implementation::~Implementation() {
	AudioManager::ErrorCheck(mpStudioSystem->unloadAll());
	AudioManager::ErrorCheck(mpStudioSystem->release());
}

void Implementation::Update(double slow) {
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

	// slow_down_factor를 적용하여 재생 속도를 조절
	for (auto& channelPair : mChannels) {
		FMOD::Channel* pChannel = channelPair.second;
		FMOD::Sound* sound = nullptr;
		if (pChannel->getCurrentSound(&sound) == FMOD_OK && sound != nullptr) {
			float defaultFrequency = 0.0f;
			// sound의 기본 주파수를 가져옵니다.
			if (sound->getDefaults(&defaultFrequency, nullptr) == FMOD_OK) {
				// slow_down_factor를 곱하여 재생 속도를 조절합니다.
				float newFrequency = defaultFrequency * static_cast<float>(slow);
				AudioManager::ErrorCheck(pChannel->setFrequency(newFrequency));
			}
		}
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
	sgpImplementation->Update(slow_down);
}

void AudioManager::LoadMusic(const std::string& filePath, const std::string& alias, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(alias);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	FMOD::Sound* pSound = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->createSound(filePath.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[alias] = pSound;
	}
}

void AudioManager::UnLoadMusic(const std::string& alias)
{
	auto tFoundIt = sgpImplementation->mSounds.find(alias);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;
	ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

FMOD::Sound* AudioManager::GetMusic(const std::string& alias) {
	auto it = sgpImplementation->mSounds.find(alias);
	if (it != sgpImplementation->mSounds.end()) {
		return it->second;
	}
	else {
		std::cerr << "Error: Music with name " << alias << " not found." << std::endl;
		return nullptr;
	}
}

float AudioManager::GetCurrentMusicTime(const std::string& alias) {
	auto it = sgpImplementation->mChannels.find(alias);
	if (it != sgpImplementation->mChannels.end()) {
		FMOD::Channel* channel = it->second;
		unsigned int currentPosition = 0;
		FMOD_RESULT result = channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
		if (ErrorCheck(result) == 0) {
			return currentPosition / 1000.0f; // 초 단위로 변환
		}
		else {
			std::cerr << "Error: Failed to get current music time." << std::endl;
		}
	}
	else {
		std::cerr << "Error: Channel with ID " << alias << " not found." << std::endl;
	}
	return 0.0f;
}

float AudioManager::GetCurrentPlayingMusicTime() {
	for (auto it = sgpImplementation->mChannels.begin(); it != sgpImplementation->mChannels.end(); ++it) {
		FMOD::Channel* channel = it->second;
		bool isPlaying = false;

		FMOD_RESULT result = channel->isPlaying(&isPlaying);
		if (ErrorCheck(result) != 0) {
			std::cerr << "Error: Failed to check playing status for channel: " << it->first << std::endl;
			continue;
		}

		if (isPlaying) {
			unsigned int currentPosition = 0;
			result = channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
			if (ErrorCheck(result) == 0) {
				return currentPosition / 1000.0f;
			}
			else {
				std::cerr << "Error: Failed to get current music time for channel: " << it->first << std::endl;
			}
		}
	}

	std::cerr << "Error: No music is currently playing." << std::endl;
	return 0.0f;
}

float AudioManager::GetMusicLength(const std::string& alias)
{
	FMOD::Sound* sound = GetMusic(alias);
	if (sound) {
		unsigned int length_ms = 0;
		sound->getLength(&length_ms, FMOD_TIMEUNIT_MS);
		return length_ms / 1000.0f; // Convert to seconds
	}
	std::cerr << "Error: Music with name " << alias << " not found." << std::endl;
	return 0.0f;
}

std::string AudioManager::GetID(const std::string& alias)
{
	// Find the sound in the channels map
	for (const auto& pair : sgpImplementation->mChannels) {
		FMOD::Channel* channel = pair.second;
		FMOD::Sound* sound;
		if (channel->getCurrentSound(&sound) == FMOD_OK) {
			if (sgpImplementation->mSounds[alias] == sound) {
				return pair.first; // Return the channel ID
			}
		}
	}
	std::cerr << "Error: Sound with name " << alias << " not found in any channel." << std::endl;
	return nullptr; // Return -1 if the sound is not found
}

std::string AudioManager::PlayMusics(const std::string& alias, const vec3& vPosition, float fVolumedB) {
	auto tFoundIt = sgpImplementation->mSounds.find(alias);
	if (tFoundIt == sgpImplementation->mSounds.end()) {
		// Load music if it hasn't been loaded yet
		std::cerr << "Error: Sound with alias " << alias << " not found. Please load it first." << std::endl;
		return "";
	}

	FMOD::Channel* pChannel = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel) {
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);

		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}

		ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		ErrorCheck(pChannel->setPaused(false));

		// Use alias as the channel ID
		sgpImplementation->mChannels[alias] = pChannel;
	}

	return alias;
}


void AudioManager::StopChannel(const std::string& alias)
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
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

void AudioManager::RestartPlayMusic(const std::string& alias)
{
	// 주어진 채널 ID를 찾기
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt == sgpImplementation->mChannels.end())
	{
		std::cerr << "Error: Channel not found!" << std::endl;
		return;
	}

	// 해당 채널을 가져오기
	FMOD::Channel* pChannel = tFoundIt->second;

	// 현재 재생 위치를 0으로 설정 (처음부터 다시 시작)
	ErrorCheck(pChannel->setPosition(0, FMOD_TIMEUNIT_MS));

	// 채널이 일시 정지 상태라면 다시 재생
	bool bIsPaused = false;
	ErrorCheck(pChannel->getPaused(&bIsPaused));
	if (bIsPaused)
	{
		ErrorCheck(pChannel->setPaused(false));
	}
}

void AudioManager::StopPlayingMusic(const std::string& alias)
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt != sgpImplementation->mChannels.end()) {
		// 채널을 일시 정지합니다.
		ErrorCheck(tFoundIt->second->setPaused(true));
	}
}


void AudioManager::Set3dListenerAndOrientation(const vec3& vPosition, const vec3& vLook, const vec3& vUp) {
	if (!sgpImplementation->mpSystem)
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	FMOD_VECTOR forward = VectorToFmod(vLook);
	FMOD_VECTOR up = VectorToFmod(vUp);

	ErrorCheck(sgpImplementation->mpSystem->set3DListenerAttributes(0, &position, nullptr, &forward, &up));
}

void AudioManager::SetChannel3dPosition(const std::string& alias, const vec3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void AudioManager::SetMode(const std::string& alias, bool spatial_on)
{
	FMOD::Channel* pChannel = nullptr;
	auto tFoundIt = sgpImplementation->mSounds.find(alias);
	ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (spatial_on)
		{
			pChannel->setMode(FMOD_3D);
		}
		else
		{
			pChannel->setMode(FMOD_2D);
		}
	}
}

void AudioManager::Set3DMode(FMOD_MODE mode) {
	for (auto& sound : sgpImplementation->mSounds) {
		sound.second->setMode(mode);
	}
}


void AudioManager::SetChannelVolume(const std::string& alias, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));

}

void AudioManager::SetMute(const std::string& alias, bool mute)
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt != sgpImplementation->mChannels.end()) {
		ErrorCheck(tFoundIt->second->setMute(mute));  // if true sound = 0
		if (mute == true)
		{
			isMute = true;
		}
		else
		{
			isMute = false;
		}
	}
}

bool AudioManager::IsPlayingMusic(const std::string& alias) const
{
	auto tFoundIt = sgpImplementation->mChannels.find(alias);
	if (tFoundIt != sgpImplementation->mChannels.end()) {
		bool bIsPlaying = false;
		ErrorCheck(tFoundIt->second->isPlaying(&bIsPlaying));
		return bIsPlaying;
	}
	return false;
}

bool AudioManager::IsAnyMusicPlaying() const {
	for (auto it = sgpImplementation->mChannels.begin(); it != sgpImplementation->mChannels.end(); ++it) {
		bool bIsPlaying = false;
		// 각 채널의 재생 여부를 확인합니다.
		if (ErrorCheck(it->second->isPlaying(&bIsPlaying)) == 0 && bIsPlaying) {
			return true;
		}
	}
	return false;
}

//bool AudioManager::IsMusicFinished(const std::string& alias) {
//	auto tFoundIt = sgpImplementation->mChannels.find(alias);
//	if (tFoundIt != sgpImplementation->mChannels.end()) {
//		bool bIsPlaying = false;
//		FMOD_RESULT result = tFoundIt->second->isPlaying(&bIsPlaying);
//		if (ErrorCheck(result) == 0) {
//			return !bIsPlaying;
//		}
//		else {
//			std::cerr << "Error: Failed to check if music is finished." << std::endl;
//		}
//	}
//	else {
//		std::cerr << "Error: Channel with alias " << alias << " not found." << std::endl;
//	}
//
//	return false;
//}

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


void AudioManager::LoadSound(const std::string& filePath, const std::string& alias, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mEffects.find(alias);
	if (tFoundIt != sgpImplementation->mEffects.end())
		return;
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	FMOD::Sound* pSound = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->createSound(filePath.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mEffects[alias] = pSound;
	}
}

std::string AudioManager::PlaySound(const std::string& alias, const vec3& vPosition, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mEffects.find(alias);
	if (tFoundIt == sgpImplementation->mEffects.end()) {
		std::cerr << "Error: Effect with alias " << alias << " not found. Please load it first." << std::endl;
		return "";
	}

	FMOD::Channel* pChannel = nullptr;
	ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel) {
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		ErrorCheck(pChannel->setPaused(false));

		sgpImplementation->mEffectChannels[alias] = pChannel;
	}
	return alias;
}

void AudioManager::StopSound(const std::string& alias)
{
	auto tFoundIt = sgpImplementation->mEffectChannels.find(alias);
	if (tFoundIt != sgpImplementation->mEffectChannels.end()) {
		ErrorCheck(tFoundIt->second->stop());
		sgpImplementation->mEffectChannels.erase(tFoundIt);
	}
}