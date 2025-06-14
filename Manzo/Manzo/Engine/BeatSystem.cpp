#include "BeatSystem.h"
#include "Engine.h"
#include <iostream>

constexpr double sync = 0.29;

Beat::Beat(AudioManager* audioMgr)
    : audio(audioMgr)
{
    fixed_duration = 60.0 / BPM;
    duration;
    delay_duration = fixed_duration / 4;
    current_delay_duration = delay_duration;
    total_music_length = audio->GetMusicLength("Level1_bgm");
    judge_offset = fixed_duration * 0.4;
}

void Beat::LoadMusicToSync(std::string _music_name)
{
    music_name = _music_name;
    playing = false;
}
//0.29

void Beat::ResetCalibration() {
    user_calibration = 0;
}
int asdfasdf = 0;
void Beat::Update(double dt)
{
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

    if (current_delay_duration + user_calibration <= time_taken) { // delay count
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
        on_beat_timer = ON_BEAT_WINDOW;
    }

    if (beat_count >= 4) {
        ++bar_count;
        beat_count = 0;
    }

    double diff = std::abs(time_taken + user_calibration - duration);
    if (diff <= judge_offset || on_beat_timer > 0.0)
        is_on_beat = true;
    else
        is_on_beat = false;

    on_beat_timer = std::max(0.0, on_beat_timer - dt);

    if (Engine::GetGameStateManager().GetStateName() == "Tutorial") {
        CollectCaliData();
    }
    //std::cout << "Time Taken: " << time_taken
    //    << ", Duration: " << duration
    //    << ", Current delay duration: " << current_delay_duration
    //    << ", Beat Count: " << beat_count
    //    << " beat: " << beat << std::endl;

        //if (beat)
    //    std::cout << asdfasdf++ << "fixed : " << fixed_duration << " duration : " << duration << " - on beat\n";
    //if (beat) std::cout << "beat @ " << time_taken << '\n';
    //std::cout << (is_on_beat ? "ON\n" : "OFF\n");
    time_taken += dt;
}

bool Beat::GetBeat()
{
    return beat;
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

    // ���� ���� �� ����
    std::vector<double> sorted = calibrations;
    std::sort(sorted.begin(), sorted.end());

    double median;
    size_t size = sorted.size();

    if (size % 2 == 0) {
        // ¦������ ���: �߾� �� ���� ���
        median = (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
    }
    else {
        // Ȧ������ ���: ��� ��
        median = sorted[size / 2];
    }

    //std::cout << "Median calibration value: " << median << std::endl;
    user_calibration = median;
    real_calibration = median;
    auto& saveData = Engine::GetSaveDataManager().GetSaveData();
    saveData.user_calibration = user_calibration; // newValue�� double Ÿ��

    Engine::GetSaveDataManager().UpdateSaveData(saveData);
    // ���⼭ median ���� ���� ������ ����ϸ� �˴ϴ�.
}


void Beat::SetBPM(int set_BPM)
{
    BPM = set_BPM;
    fixed_duration = 60.0 / BPM;
    duration = fixed_duration;
    delay_duration = duration / 4;
    current_delay_duration = delay_duration;
    judge_offset = fixed_duration * 0.4;
    time_taken = 0;
    bar_count = 0;
    beat_count = 0;
    delay_count = 0;
    beat = false;
    is_on_beat = false;
    playing = false;
    //music_name.clear();

    //maybe wrong
    double ratio = 100 / set_BPM;
    user_calibration = real_calibration * ratio;
}

void Beat::CleartoOriginal() {

    fixed_duration = 60.0 / BPM;
    duration = fixed_duration;
    delay_duration = fixed_duration / 4;
    current_delay_duration = delay_duration;
    judge_offset = fixed_duration * 0.4;

    time_taken = 0;
    bar_count = 0;
    beat_count = 0;
    delay_count = 0;

    beat = false;
    is_on_beat = false;
    playing = false;
    //music_name.clear();
}
