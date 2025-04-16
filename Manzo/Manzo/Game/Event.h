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
    std::function<bool()> trigger;  // ���� ����
    std::function<void()> action;   // ���� ���� �� ������ �ڵ�
    bool triggered;                 // �̹� ����ƴ��� �÷���
};


//// 1) �̺�Ʈ ���� & ���
//// - �÷��̾ ����⸦ 5���� �̻� ������ ���� ����
//eventMgr.AddEvent(Event(
//    []() { return playerFishCount >= 5; },  // trigger
//    []() { GiveFishReward(); }              // action
//));