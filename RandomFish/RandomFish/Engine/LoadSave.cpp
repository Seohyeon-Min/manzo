#include "LoadSave.h"
#include <fstream>

void CS230::LoadSave::Load(int& previous_day, int& previous_money, bool previous_drinks[num_drinks]) const
{
	std::ifstream in_file(savefile);

	if (in_file.is_open() == false) {
		throw std::runtime_error("Failed to Load file");
	}

	std::string text;

	while (in_file >> text) {
		if (text == "Day") {
			in_file >> previous_day;
		}
		else if (text == "Money") {
			in_file >> previous_money;
		}
		else if (text == "Drinks") {
			for (int i = 0; i < num_drinks; ++i) {
				in_file >> previous_drinks[i];
			}
		}
	}
}

void CS230::LoadSave::Save(const int current_day, const int current_money, const bool current_drinks[num_drinks])
{
	std::ofstream in_file(savefile);
	if (in_file.is_open() == false) {
		throw std::runtime_error("Failed to Save file");
	}

	in_file << "Day " << current_day << std::endl;
	in_file << "Money " << current_money << std::endl;
	in_file << "Drinks ";
	for (int i = 0; i < num_drinks; ++i) {
		in_file << current_drinks[i] << " ";
	}
}

//Initialize value for new game
void CS230::LoadSave::Clear() 
{
	int initial_day = 1;
	int initial_money = 0;
	bool initial_drinks[num_drinks] = { false };

	Save(initial_day, initial_money, initial_drinks);
}
