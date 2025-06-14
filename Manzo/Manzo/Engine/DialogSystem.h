#pragma once
#include <memory>
#include "../Game/DialogBox.h"

class DialogSystem {
public:
    static DialogSystem& Instance(); // ╫л╠шео

    void SetDialog(std::shared_ptr<Dialog> dlg);
    std::shared_ptr<Dialog> GetDialog();
    void ClearDialog();

private:
    DialogSystem() = default;
    std::shared_ptr<Dialog> current_dialog;
};