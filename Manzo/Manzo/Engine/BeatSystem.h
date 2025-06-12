#pragma once

#include "Component.h"
#include "AudioManager.h"

class Beat {
public:
    Beat(AudioManager* audioMgr);
    void LoadMusicToSync(std::string music_name);
    void ResetCalibration();
    void Update(double dt);
    bool GetBeat() { return beat; }
    bool GetIsOnBeat() { return is_on_beat; }
    void SetBPM(int set_BPM);
    int GetBarCount() { return bar_count; }
    int GetDelayCount() { return delay_count; }
    double GetFixedDuration() { return fixed_duration; }
    void CollectCaliData();
    void CalculateCali();
    void CleartoOriginal();
    double GetUserCali() { return user_calibration; }
    const double GetLastCali() {return calibrations.back();}
    void SetUserCali(double cali) { user_calibration = cali; }
    void SetPause(bool pause_) { playing = pause_; }

private:
    int BPM = 100;
    int bar_count = 0;
    int beat_count = 0;
    int delay_count = 0;
    double duration = 0;
    double fixed_duration = 0;
    double delay_duration = 0;
    double current_delay_duration = 0;
    double time_taken = 0;
    double total_music_length = 0;
    double right_time_for_calibration;
    static constexpr double judge_offset = 0.2;
    bool beat = true;
    bool is_on_beat = false;
    bool playing = false;
    AudioManager* audio;
    int channel_id = 0;
    std::string music_name;

    std::vector<double> calibrations;
    int calibrations_cnt = 0;
    double user_calibration = 0;
};
