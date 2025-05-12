#include "BeatSystem.h"
#include "Engine.h"
#include <iostream>

constexpr double sync = 0.29;

Beat::Beat(AudioManager* audioMgr)
    : audio(audioMgr)
{
    fixed_duration = 60.0 / BPM;
    duration; 
    delay_duration = fixed_duration / 4 ;
    current_delay_duration = delay_duration ;
    total_music_length = audio->GetMusicLength("Level1_bgm");
}

void Beat::LoadMusicToSync(std::string _music_name)
{
    music_name =_music_name;
    playing = false;
}
//0.29

void Beat::ResetCalibration() {
    user_calibration = 0;
}

void Beat::Update(double dt)
{
    time_taken += dt;

    if (!playing)
    {
        audio->PlayMusics(music_name);
        playing = true;
        time_taken = 0;
        //std::cout << "Now Music:: " << Engine::GetAudioManager().GetCurrentMusicTime(music_name) << std::endl;
    }

    if (beat) {
        beat = false;
    }

    if (current_delay_duration + user_calibration <= time_taken ) { // delay count
        if (delay_count >= 16) {
            delay_count = 0;
        }
        delay_count++;
        current_delay_duration += delay_duration;
    }

    if (duration + user_calibration <= time_taken) { // right beat
        beat = true; // Beat detected

        right_time_for_calibration = time_taken;

        beat_count++;
        duration += fixed_duration; // Update duration for the next beat
    }

    if (beat_count >= 4) {
        ++bar_count;
        beat_count = 0;
    }

    if ((judge_offset >= time_taken + user_calibration || duration - judge_offset <= time_taken + user_calibration)) { // judge_offset
        if (!is_on_beat)
            is_on_beat = true;
    }
    else {
        if (is_on_beat)
            is_on_beat = false;
    }

    if (Engine::GetGameStateManager().GetStateName() == "Tutorial") {
        CollectCaliData();
    }
    //std::cout << "Time Taken: " << time_taken
    //    << ", Duration: " << duration
    //    << ", Current delay duration: " << current_delay_duration
    //    << ", Beat Count: " << beat_count
    //    << " beat: " << beat << std::endl;
}

void Beat::CollectCaliData()
{
    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        double interval = time_taken - right_time_for_calibration;
        if (interval >= fixed_duration / 2) {
            interval = time_taken - (right_time_for_calibration + fixed_duration);
        }
        calibrations.push_back(interval);
        //std::cout << interval << std::endl;
        calibrations_cnt++;
        //CalculateCali();
    }
}

void Beat::CalculateCali()
{
    if (calibrations.empty()) {
        std::cout << "Calibration data is empty." << std::endl;
        return;
    }

    // 벡터 복사 후 정렬
    std::vector<double> sorted = calibrations;
    std::sort(sorted.begin(), sorted.end());

    double median;
    size_t size = sorted.size();

    if (size % 2 == 0) {
        // 짝수개일 경우: 중앙 두 값의 평균
        median = (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
    }
    else {
        // 홀수개일 경우: 가운데 값
        median = sorted[size / 2];
    }

    //std::cout << "Median calibration value: " << median << std::endl;
    user_calibration = median;
    // 여기서 median 값을 게임 보정에 사용하면 됩니다.
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
