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
	AudioManager::ErrorCheck(mpSystem->createChannelGroup("SFX", &mSFXGroup));

	int driverCount = 0;
	AudioManager::ErrorCheck(mpSystem->getNumDrivers(&driverCount));
	std::cout << "Available audio drivers: " << driverCount << std::endl;


	FMOD::ChannelGroup* masterGroup = nullptr;
	AudioManager::ErrorCheck(mpSystem->getMasterChannelGroup(&masterGroup));


	AudioManager::ErrorCheck(masterGroup->addGroup(mSFXGroup));
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

void AudioManager::LoadMusic(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
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

void AudioManager::UnLoadMusic(const std::string& strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;
	ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

FMOD::Sound* AudioManager::GetMusic(const std::string& strMusicName) {
	auto it = sgpImplementation->mSounds.find(strMusicName);
	if (it != sgpImplementation->mSounds.end()) {
		return it->second;
	}
	else {
		std::cerr << "Error: Music with name " << strMusicName << " not found." << std::endl;
		return nullptr;
	}
}

float AudioManager::GetCurrentMusicTime(int nChannelId) {
	auto it = sgpImplementation->mChannels.find(nChannelId);
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
		std::cerr << "Error: Channel with ID " << nChannelId << " not found." << std::endl;
	}
	return 0.0f;
}


int AudioManager::GetID(const std::string& soundName)
{
	// Find the sound in the channels map
	for (const auto& pair : sgpImplementation->mChannels) {
		FMOD::Channel* channel = pair.second;
		FMOD::Sound* sound;
		if (channel->getCurrentSound(&sound) == FMOD_OK) {
			if (sgpImplementation->mSounds[soundName] == sound) {
				return pair.first; // Return the channel ID
			}
		}
	}
	std::cerr << "Error: Sound with name " << soundName << " not found in any channel." << std::endl;
	return -1; // Return -1 if the sound is not found
}

int AudioManager::PlayMusics(const std::string& strSoundName, const vec3& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		//LoadMusic(strSoundName);
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

void AudioManager::RestartPlayMusic(int nChannelId)
{
	// 주어진 채널 ID를 찾기
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
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

void AudioManager::StopPlayingMusic(int nChannelId)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
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

void AudioManager::SetChannel3dPosition(int nChannelId, const vec3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void AudioManager::SetMode(const std::string& strSoundName, bool spatial_on)
{
	FMOD::Channel* pChannel = nullptr;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
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


void AudioManager::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

void AudioManager::SetMute(int nChannelId, bool mute)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
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

bool AudioManager::IsPlayingMusic(int nChannelId) const
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
