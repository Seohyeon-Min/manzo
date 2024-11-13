#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Component.h"
#include "../Engine/AudioManager.h"

class Beat : public CS230::Component {
public:
    Beat();
    void Update(double dt);
    bool GetBeat() { return beat; }
    bool GetIsOnBeat() { return is_on_beat; }
    void SetBPM(int set_BPM);
    int GetBarCount() { return bar_count; }
    int GetDelayCount() { return delay_count; }

private:
    int BPM = 100;
    int bar_count = 0;
    int beat_count = 0;
    int delay_count = 0;
    double duration = 0;
    double delay_duration = 0;
    double current_delay_duration = 0;
    double time_taken = 0;
    static constexpr double judge_offset = 0.20;
    bool beat = false;
    bool is_on_beat = false;
    AudioManager* audio;
};
