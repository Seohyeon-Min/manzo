#pragma once

#include "UI.h"
#include <vector>
#include <memory>

class UIManager : public CS230::Component{
public:
    // UI 요소 추가
    void AddUI(std::unique_ptr<UI> ui) {
        ui_elements.push_back(std::move(ui));
    }

    // 모든 UI 요소의 AddDrawCall을 호출
    void Update(double dt) override {
        for (auto& ui : ui_elements) {
            ui->Update(dt);
        }
    }

    void AddDrawCalls() {
        for (auto& ui : ui_elements) {
            ui->AddDrawCall();
        }
    }

    ~UIManager() {
        ui_elements.clear();
    }

private:
    std::vector<std::unique_ptr<UI>> ui_elements;  // UI 요소를 보관하는 벡터
};
