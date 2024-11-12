#pragma once

#include "UI.h"
#include <vector>
#include <memory>

class UIManager : public CS230::Component{
public:
    // UI ��� �߰�
    void AddUI(std::unique_ptr<UI> ui) {
        ui_elements.push_back(std::move(ui));
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

    ~UIManager() {
        ui_elements.clear();
    }

private:
    std::vector<std::unique_ptr<UI>> ui_elements;  // UI ��Ҹ� �����ϴ� ����
};
