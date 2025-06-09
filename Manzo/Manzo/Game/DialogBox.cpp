#include "DialogBox.h"
#include "../Engine/Engine.h"

Dialog::Dialog(vec2 start_position)
    : GameObject(start_position), currentIndex(0), elapsedTime(0.0), typingSpeed(0.05), IsTyping(false) {

    dialog = new JsonParser_dialog("en");
    AddGOComponent(new Sprite("assets/images/none.spt", this));
    DialogBox = Engine::GetTextureManager().Load("assets/images/temp_dial.png");
    Box_effect = Engine::GetTextureManager().Load("assets/images/box_effect.png");
    Me = Engine::GetTextureManager().Load("assets/images/character/Me.png");
    Sellerkeeper_1 = Engine::GetTextureManager().Load("assets/images/character/shopkeeper.png");
    Perry = Engine::GetTextureManager().Load("assets/images/character/perry.png");

    back_matrix = mat3::build_translation({ 0, -(Engine::window_height) / 3 })
        * mat3::build_scale(1.f)
        * mat3::build_rotation(3.141592f / 1.0f);

    effect_matrix = mat3::build_translation({ 0, (-(Engine::window_height) / 2) + 50 })
        * mat3::build_scale(1.f)
        * mat3::build_rotation(0.0f);

    me_matrix = mat3::build_translation({ ((Engine::window_width) / 3) + 60, (-(Engine::window_height) / 3) + 50 })
        * mat3::build_scale(0.2f)
        * mat3::build_rotation(0.0f);

    seller_martix = mat3::build_translation({ -((Engine::window_width) / 3) - 60, (-(Engine::window_height) / 3) + 50 })
        * mat3::build_scale(0.2f)
        * mat3::build_rotation(0.0f);

    perry_martix = mat3::build_translation({ -((Engine::window_width) / 3) - 60, (-(Engine::window_height) / 3) + 50 })
        * mat3::build_scale(0.2f)
        * mat3::build_rotation(0.0f);

    boxposition = { 50, 100 };
}

void Dialog::LoadDialogGroup(const std::string& group_id, double Speed) {
    is_Finished = false;
    currentDialogGroup = dialog->GetDialogGroup(group_id);
    currentLineIndex = 0;
    typingSpeed = Speed;
    StartLine(currentLineIndex);
    isVisible = true;
}

void Dialog::LoadRandomDialog(const std::string& group_id, double Speed) {
    const auto& lines = dialog->GetDialogGroup(group_id);
    if (lines.empty()) return;

    int randIndex = rand() % lines.size();
    const auto& [charName, textLine] = lines[randIndex];

    character = charName;
    fullText = textLine;
    displayedText = "";
    currentIndex = 0;
    elapsedTime = 0.0;
    typingSpeed = Speed;
    IsTyping = true;
    isVisible = true;
}

void Dialog::StartLine(int index) {
    if (index >= static_cast<int>(currentDialogGroup.size())) {
        IsTyping = false;
        return;
    }

    const auto& [charName, textLine] = currentDialogGroup[index];
    character = charName;
    fullText = textLine;
    displayedText = "";
    currentIndex = 0;
    elapsedTime = 0.0;
    IsTyping = true;
}

//bool Dialog::IsFinished() const {
//    return !IsTyping &&
//        currentLineIndex == static_cast<int>(currentDialogGroup.size()) - 1 &&
//        currentIndex >= static_cast<int>(fullText.size());
//}


void Dialog::NextLine() {
    if (IsTyping) {
        displayedText = fullText;
        currentIndex = static_cast<int>(fullText.size());
        IsTyping = false;
    }
    else if (currentLineIndex + 1 < static_cast<int>(currentDialogGroup.size())) {
        currentLineIndex++;
        StartLine(currentLineIndex);
    }
    else {
        Hide();
    }
}


void Dialog::Update(double dt) {
    if (!IsTyping && IsFinished()) {
        Hide();  
        return;
    }

    if (!IsTyping) return;

    elapsedTime += dt;
    while (elapsedTime >= typingSpeed && currentIndex < fullText.size()) {
        elapsedTime -= typingSpeed;
        displayedText += fullText[currentIndex];
        currentIndex++;

        if (currentIndex >= fullText.size()) {
            IsTyping = false;
            break;
        }
    }
}



void Dialog::Hide() {
    isVisible = false;
    IsTyping = false;
    displayedText = "";
    fullText = "";
    currentDialogGroup.clear(); 
    is_Finished = true;
    character = "";
    currentIndex = 0;
}


void Dialog::Draw() {
    if (!isVisible) return;

    DrawCall draw_call_box = { DialogBox, &back_matrix, Engine::GetShaderManager().GetDefaultShader() };
    DrawCall draw_call_effect = { Box_effect, &effect_matrix, Engine::GetShaderManager().GetDefaultShader() };

    SetDrawSetting(draw_call_box);
    SetDrawSetting(draw_call_effect);
    GameObject::Draw(draw_call_effect);
    GameObject::Draw(draw_call_box);

    if (character == "Me") {
        DrawCall draw_me = { Me, &me_matrix, Engine::GetShaderManager().GetDefaultShader() };
        SetDrawSetting(draw_me);
        GameObject::Draw(draw_me);
    }
    else if (character == "Sellerkeeper") {
        DrawCall draw_seller = { Sellerkeeper_1, &seller_martix, Engine::GetShaderManager().GetDefaultShader() };
        SetDrawSetting(draw_seller);
        GameObject::Draw(draw_seller);
    }
    else if (character == "Perry") {
        DrawCall draw_perry = { Perry, &perry_martix, Engine::GetShaderManager().GetDefaultShader() };
        SetDrawSetting(draw_perry);
        GameObject::Draw(draw_perry);
    }
    if (!displayedText.empty()) {
        Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT,
            displayedText.c_str(), { -175.f,-120.f }, 0.05f, { 1.0f, 1.0f, 1.0f }, 1.f);
        Engine::GetFontManager().PrintText(FontType::AlumniSans_Medium, FontAlignment::LEFT,
            character.c_str(), { -175.f, -100.f }, 0.08f, { 1.0f, 1.0f, 1.0f }, 1.f);
    }
}



void Dialog::Unload() {
    DialogBox = nullptr;
    Box_effect = nullptr;
    Me = nullptr;
    Sellerkeeper_1 = nullptr;
    Perry = nullptr;
    dialog = nullptr;
}

void Dialog::SetDrawSetting(DrawCall &draw_call)
{
    draw_call.settings.do_blending = true;
    draw_call.settings.is_camera_fixed = true;
    draw_call.sorting_layer = DrawLayer::DrawDialog;
}
