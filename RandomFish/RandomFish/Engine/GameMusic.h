#pragma once
#include "Component.h"
#include <filesystem>
#include <raylib.h>

namespace CS230 {
	class GameMusic {
	public:
		GameMusic();
		~GameMusic() { CloseAudioDevice(); }
		void Load(const std::filesystem::path& music_file, float given_volume =1.0f);
		void Start();
		void Update();
		void Pause();
		void Resume();
		void Stop();
		void SetVolume(float volume);
		void Unload();

		bool IsMusicPlaying()const;
		float GetVolume()const;

	private:
		Music music;
		float volume;
	};
}