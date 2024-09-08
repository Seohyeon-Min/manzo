#pragma once
#include "Component.h"
#include <filesystem>
#include <raylib.h>

namespace CS230 {
	class GameSound {
	public:
		GameSound(const std::filesystem::path& sound_file, float volume);
		~GameSound();

		void Load(const std::filesystem::path& sound_file, float given_volume);
		void Start();
		void Pause();
		void Resume();
		void Stop();
		void SetVolume(float volume);
		void Unload();
		bool Playing()const;

	private:
		Sound sound;
		float volume;
	};
}