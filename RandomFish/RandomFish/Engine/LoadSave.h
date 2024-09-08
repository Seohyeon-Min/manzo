#pragma once
#include <string>

constexpr int num_drinks = 10;

namespace CS230 {
	class LoadSave {
	public:
		static LoadSave& Instance() {
			static LoadSave instance;
			return instance;
		}

		const std::string savefile = "Save.txt";
		void Load(int& previous_day, int& previous_money, bool previous_drinks[num_drinks])const;
		void Save(const int current_day, const int current_money, const bool current_drinks[num_drinks]);
		void Clear();

	private:
		int day;
		int money;
		bool unlocked_drinks[num_drinks];
	};
}