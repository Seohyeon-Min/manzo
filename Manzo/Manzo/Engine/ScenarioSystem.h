#pragma once
#include <memory>

class ScenarioSystem {
public:
    void LoadMode2Scenarios();      // �� ���� ȣ��
    void Mode2Update();

private:
    bool triggered = false;
};