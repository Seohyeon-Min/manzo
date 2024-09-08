#include "BeatSystem.h"
#include <iostream>
Beat::Beat()
{
	duration = BPM / 60;
}

void Beat::Update(double dt)
{
	time_taken += dt;

	if (beat) {
		beat = false;
	}
	if (duration <= time_taken) { // right beat
		std::cout << "beat" << std::endl;
		beat = true;
		time_taken = 0;
	}
	if (beat_offset >= time_taken || duration - beat_offset <= time_taken && !is_on_beat) { // judge_offset
		is_on_beat = true;
	}
	else {
		is_on_beat = false;
	}
}
