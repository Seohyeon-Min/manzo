#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Component.h"

class Beat : public CS230::Component {
public:
    Beat();
    void Update(double dt);
    bool GetBeat() { return beat; }
    bool GetIsOnBeat() { return is_on_beat; }
private:
    static constexpr double BPM = 70;
    double duration = 0;
    double time_taken = 0;
    bool beat = false;
    bool is_on_beat = false;
    static constexpr double beat_offset = 0.3;
};
