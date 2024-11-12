#include "BeatSystem.h"
#include <iostream>

Beat::Beat()
{
	audio = Engine::GetGameStateManager().GetGSComponent<AudioManager>();
	duration = 60.0 / BPM;
	delay_duration = duration / 4;
	current_delay_duration = delay_duration;
}

void Beat::Update(double dt)
{
	//std::cout << is_on_beat << std::endl;
	//audio->GetCurrentMusicTime();

	time_taken += dt;
	if (beat_count >= 4) {
		++bar_count;
		beat_count = 0;
	}

	if (beat) {
		beat = false;
	}

	if (current_delay_duration <= time_taken) { // delay count
		++delay_count;
		if (delay_count >= 16) {
			delay_count = 0;
		}
		current_delay_duration += delay_duration;
	}

	if (duration <= time_taken ) { // right beat
		beat = true;
		beat_count++;
		time_taken = 0 ;
		current_delay_duration = delay_duration;
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

void Beat::SetBPM(int set_BPM)
{
	BPM = set_BPM; 
	duration = 60.0 / BPM; 
	delay_duration = duration / 4;
	current_delay_duration = 0;
	time_taken = 0;
	bar_count = 0;
	beat_count = 0;
	delay_count = 0;
	beat = false;
	is_on_beat = false;
}
