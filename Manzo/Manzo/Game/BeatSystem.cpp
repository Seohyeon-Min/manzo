#include "BeatSystem.h"
#include <iostream>

double sync = 0.29;

Beat::Beat()
{
    audio = Engine::GetGameStateManager().GetGSComponent<AudioManager>();
    fixed_duration = 60.0 / BPM;
    duration += sync; 
    delay_duration = fixed_duration / 4 ;
    current_delay_duration = delay_duration ;
}

//0.29

void Beat::Update(double dt)
{
    //std::cout << is_on_beat << std::endl;
    //audio->GetCurrentMusicTime();

    double current_music_time = Engine::GetAudioManager().GetCurrentMusicTime();

    if (current_music_time > 0.0) { 
        time_taken = current_music_time + sync; 
    }

    std::cout << time_taken << " : " << delay_count << std::endl;

    if (beat) {
        beat = false;
    }

    if (current_delay_duration <= time_taken) { // delay count
        if (delay_count >= 16) {
            delay_count = 0;
        }
        delay_count++;
        current_delay_duration += delay_duration;
    }

    if (duration <= time_taken) { // right beat // music starts before the game loaded
        beat = true; // 비트 감지

        beat_count++;
        duration = duration + fixed_duration; // 다음 비트를 위해 duration 업데이트
    }

    if (beat_count >= 4) {
        ++bar_count;
        beat_count = 0;
    }

    if ((judge_offset >= time_taken || duration - judge_offset <= time_taken)) { // judge_offset
        if (!is_on_beat)
            is_on_beat = true;
    }
    else {
        if (is_on_beat)
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
