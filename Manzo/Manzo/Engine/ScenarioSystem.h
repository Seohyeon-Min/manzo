#pragma once
#include <memory>

class ScenarioSystem {
public:
    void LoadMode2Scenarios();      // 한 번만 호출
    void Mode2Update();

private:
    bool triggered = false;
};