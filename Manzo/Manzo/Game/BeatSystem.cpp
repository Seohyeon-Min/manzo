#include "BeatSystem.h"
#include <iostream>

Beat::Beat()
{
	audio = Engine::GetGameStateManager().GetGSComponent<AudioManager>();
	duration = 60.0 / BPM;
}

void Beat::Update(double dt)
{
	
	audio->GetCurrentMusicTime();
	time_taken += dt;
	
	std::cout << is_on_beat << std::endl;

	if (beat) {
		beat = false;
	}
	if (duration <= time_taken ) { // right beat
		std::cout << "beat" << std::endl;
		beat = true;
		time_taken = 0 ;
	}
	if ((judge_offset >= time_taken || duration - judge_offset <= time_taken)) { // judge_offset
		if (!is_on_beat)
			is_on_beat = true;
	}
	else {
		if(is_on_beat)
			is_on_beat = false;
	}
}
