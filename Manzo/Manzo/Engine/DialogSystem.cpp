#include "DialogSystem.h"

DialogSystem::DialogSystem()
{
    dialog = new Dialog({0,0});
}

DialogSystem::~DialogSystem()
{
    Unload();
    delete dialog;
}

Dialog& DialogSystem::GetDialog()
{
    return *dialog;
}

void DialogSystem::Unload()
{
    if(dialog) {
        dialog->Unload();
    }
}
