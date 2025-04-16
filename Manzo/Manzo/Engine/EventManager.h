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


//// 1일차 이벤트
//scenarioMgr.AddEvent(Event(
//    // trigger: day가 1일 때
//    [&]() { return currentDay == 1; },
//    // action: Day1 로직 실행 후 플래그 세팅
//    [&]() {
//        RunDay1Logic();          // 1일차 대화, 튜토리얼 등
//        currentDay = 2;          // 다음 날로 넘어가도록 설정
//    }
//));