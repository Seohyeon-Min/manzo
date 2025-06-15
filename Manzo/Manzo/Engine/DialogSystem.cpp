#include "DialogSystem.h"
#include "../Game/DialogBox.h"

void DialogSystem::SetDialog(std::shared_ptr<Dialog> dlg) {
    current_dialog = dlg;
}

std::shared_ptr<Dialog> DialogSystem::GetDialog() {
    return current_dialog;
}

void DialogSystem::ClearDialog() {
    current_dialog.reset();
}