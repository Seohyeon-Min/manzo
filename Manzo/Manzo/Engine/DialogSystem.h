#pragma once
#include "../Game/DialogBox.h"

class DialogSystem {
public:
    DialogSystem();
    ~DialogSystem();

    Dialog& GetDialog();
    void Unload();

private:
    Dialog* dialog;
};
