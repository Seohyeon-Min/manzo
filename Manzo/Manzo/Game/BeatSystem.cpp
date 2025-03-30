#include "BeatSystem.h"
#include <iostream>

constexpr double sync = 0.29;

Beat::Beat()
{
    audio = &Engine::GetAudioManager();
    fixed_duration = 60.0 / BPM;
    duration; 
    delay_duration = fixed_duration / 4 ;
    current_delay_duration = delay_duration ;
    total_music_length = Engine::GetAudioManager().GetMusicLength("Level1_bgm");
}

void Beat::LoadMusicToSync(std::string _music_name)
{
    music_name =_music_name;
    playing = false;
}
//0.29

void Beat::Update(double dt)
{
    time_taken += dt;

    if (!playing)
    {
        Engine::GetAudioManager().PlayMusics(music_name);
        playing = true;
        time_taken = 0;
        //std::cout << "Now Music:: " << Engine::GetAudioManager().GetCurrentMusicTime(music_name) << std::endl;
    }

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

    if (duration <= time_taken) { // right beat
        beat = true; // Beat detected

        beat_count++;
        duration += fixed_duration; // Update duration for the next beat
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

    //std::cout << "Time Taken: " << time_taken
    //    << ", Duration: " << duration
    //    << ", Current delay duration: " << current_delay_duration
    //    << ", Beat Count: " << beat_count
    //    << " beat: " << beat << std::endl;
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
    playing = false;
    //music_name.clear();
}

void Beat::CleartoOriginal() {
    fixed_duration = 60.0 / BPM;
    duration = fixed_duration;
    delay_duration = fixed_duration / 4;
    current_delay_duration = delay_duration;

    time_taken = 0;
    bar_count = 0;
    beat_count = 0;
    delay_count = 0;

    beat = false;
    is_on_beat = false;
    playing = false;
    //music_name.clear();
}
