#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Component.h"
#include "../Engine/AudioManager.h"

class Beat : public Component {
public:
    Beat();
    void LoadMusicToSync(std::string music_name);
    void Update(double dt);
    bool GetBeat() { return beat; }
    bool GetIsOnBeat() { return is_on_beat; }
    void SetBPM(int set_BPM);
    int GetBarCount() { return bar_count; }
    int GetDelayCount() { return delay_count; }
    double GetFixedDuration() { return fixed_duration; }
    void CleartoOriginal();

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
    static constexpr double judge_offset = 0.2;
    bool beat = false;
    bool is_on_beat = false;
    bool music_started = false;
    AudioManager* audio;
    int channel_id = 0;
    std::string music_name;
};
