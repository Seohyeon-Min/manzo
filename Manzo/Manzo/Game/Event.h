#pragma once
#include <functional>

class Event {
public:
    Event(std::function<bool()> triggerFunc, std::function<void()> actionFunc)
        : trigger(triggerFunc), action(actionFunc), triggered(false)
    { }

    void Update() {
        if (!triggered && trigger()) {
            action();
            triggered = true;
        }
    }

    void Reset() {
        triggered = false;
    }

private:
    std::function<bool()> trigger;  // 실행 조건
    std::function<void()> action;   // 조건 만족 시 실행할 코드
    bool triggered;                 // 이미 실행됐는지 플래그
};


//// 1) 이벤트 생성 & 등록
//// - 플레이어가 물고기를 5마리 이상 잡으면 보상 지급
//eventMgr.AddEvent(Event(
//    []() { return playerFishCount >= 5; },  // trigger
//    []() { GiveFishReward(); }              // action
//));