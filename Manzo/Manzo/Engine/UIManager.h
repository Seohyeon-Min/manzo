#pragma once

#include "UI.h"
#include <vector>
#include <memory>

class UIManager : public CS230::Component{
public:
    // UI ��� �߰�
    void AddUI(std::shared_ptr<UI> ui) {
        ui_elements.push_back(ui);
    }

    // ��� UI ����� AddDrawCall�� ȣ��
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

private:
    std::vector<std::shared_ptr<UI>> ui_elements;  // UI ��Ҹ� �����ϴ� ����
};
