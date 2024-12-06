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
}

void Beat::LoadMusicToSync(std::string music_name)
{
    music = audio->GetMusic(music_name);
}
//0.29

void Beat::Update(double dt)
{

    //std::cout << is_on_beat << std::endl;
    //audio->GetCurrentMusicTime();
    if (!music_started && music) {
        // 비트 시스템이 준비되었을 때 음악 재생 시작
        audio->PlayMusic(music ,-1); // 무한 루프 재생
        Mix_VolumeMusic(0);
        music_started = true;       // 음악 재생 상태 갱신
    }

    double current_music_time = audio->GetCurrentMusicTime();

    if (current_music_time > 0.0) { 
        time_taken = current_music_time + sync; 
    }

    //std::cout << time_taken << " : " << is_on_beat << std::endl;

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
    music_started = false;
    music = nullptr;
}
