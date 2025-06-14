#include "DialogSystem.h"

DialogSystem& DialogSystem::Instance() {
    static DialogSystem instance;
    return instance;
}

void DialogSystem::SetDialog(std::shared_ptr<Dialog> dlg) {
    current_dialog = dlg;
}

std::shared_ptr<Dialog> DialogSystem::GetDialog() {
    return current_dialog;
}

void DialogSystem::ClearDialog() {
    current_dialog.reset();
}