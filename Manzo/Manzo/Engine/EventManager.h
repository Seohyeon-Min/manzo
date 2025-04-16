#pragma once
#include "../Game/Event.h"
#include <vector>

class EventManager {
public:
    void AddEvent(const Event& event) {
        events.push_back(event);
    }

    void Update() {
        for (auto& event : events) {
            event.Update();
        }
    }

    void ResetAll() {
        for (auto& event : events) {
            event.Reset();
        }
    }

private:
    std::vector<Event> events; 
};


//// 1���� �̺�Ʈ
//scenarioMgr.AddEvent(Event(
//    // trigger: day�� 1�� ��
//    [&]() { return currentDay == 1; },
//    // action: Day1 ���� ���� �� �÷��� ����
//    [&]() {
//        RunDay1Logic();          // 1���� ��ȭ, Ʃ�丮�� ��
//        currentDay = 2;          // ���� ���� �Ѿ���� ����
//    }
//));