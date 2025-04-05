#include "DialogBox.h"
#include "../Engine/Engine.h"

Dialog::Dialog(vec2 start_position)
    : GameObject(start_position), currentIndex(0), elapsedTime(0.0), typingSpeed(0.05), IsTyping(false) {
    AddGOComponent(new Sprite("assets/images/none.spt", this));

    dialog = new JsonParser_dialog("en");
    DialogBox = Engine::GetTextureManager().Load("assets/images/temp_dial.png");
    Box_effect = Engine::GetTextureManager().Load("assets/images/box_effect.png");
    Me = Engine::GetTextureManager().Load("assets/images/Me.png");
    Sellerkeeper_1 = Engine::GetTextureManager().Load("assets/images/shopkeeper.png");
    Perry = Engine::GetTextureManager().Load("assets/images/perry.png");

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
    currentDialogGroup = dialog->GetDialogGroup(group_id);
    currentLineIndex = 0;
    typingSpeed = Speed;
    StartLine(currentLineIndex);
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

void Dialog::NextLine() {
    if (!IsTyping && currentLineIndex + 1 < static_cast<int>(currentDialogGroup.size())) {
        currentLineIndex++;
        StartLine(currentLineIndex);
    }
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
}

void Dialog::Update(double dt) {
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

void Dialog::Draw() {
    DrawCall draw_call_box = { DialogBox, &back_matrix, Engine::GetShaderManager().GetDefaultShader()};
    DrawCall draw_call_effect = { Box_effect, &effect_matrix, Engine::GetShaderManager().GetDefaultShader() };
    DrawCall draw_call_Me = { Me, &me_matrix, Engine::GetShaderManager().GetDefaultShader() };

    SetDrawSetting(draw_call_box);
    SetDrawSetting(draw_call_effect);
    SetDrawSetting(draw_call_Me);


    if (displayedText != "") {
        GameObject::Draw(draw_call_Me);
        GameObject::Draw(draw_call_effect);
        GameObject::Draw(draw_call_box);


        Engine::GetFontManager().PrintText(FontType::Thin, displayedText.c_str(), { -175.f,-120.f },  0.05f, { 1.0f, 1.0f, 1.0f }, 1.f);
        Engine::GetFontManager().PrintText(FontType::Bold, character.c_str(), { -175.f, -100.f }, 0.08f, { 1.0f, 1.0f, 1.0f }, 1.f);
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
