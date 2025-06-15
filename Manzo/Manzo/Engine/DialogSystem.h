#pragma once
#include <memory>

class Dialog;

class DialogSystem {
public:
    DialogSystem() = default;
    void SetDialog(std::shared_ptr<Dialog> dlg);
    std::shared_ptr<Dialog> GetDialog();
    void ClearDialog();

private:
    std::shared_ptr<Dialog> current_dialog;
};