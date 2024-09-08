#include "BeatSystem.h"
#include <iostream>
Beat::Beat()
{
	duration = 60.0 / BPM;
}

void Beat::Update(double dt)
{
	time_taken += dt;

	if (beat) {
		beat = false;
	}

	// right beat
	if (duration <= time_taken) { 
		std::cout << "beat" << std::endl;
		beat = true;
		time_taken = 0;
	}

	// judge offset
	if ((beat_offset >= time_taken || duration - beat_offset <= time_taken) && !is_on_beat) { 
		is_on_beat = true;
	}
	else {
		is_on_beat = false;
	}
}
