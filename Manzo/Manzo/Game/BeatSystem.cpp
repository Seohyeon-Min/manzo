#include "BeatSystem.h"
#include <iostream>

Beat::Beat()
{
	audio = Engine::GetGameStateManager().GetGSComponent<AudioManager>();
	duration = 60.0 / BPM;
}

void Beat::Update(double dt)
{
	//std::cout << is_on_beat << std::endl;
	audio->GetCurrentMusicTime();
	time_taken += dt;

	if (beat_count == 4) {
		bar_count++;
		beat_count = 0;
	}

	if (beat) {
		beat = false;
	}
	if (duration <= time_taken ) { // right beat
		beat = true;
		beat_count++;
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
