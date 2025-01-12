#pragma once
#include "..\Engine/Engine.h"
#include <string>
#include <vector>
#include <iostream>
#include "..\Game\Ship.h"
#include "../Engine/Component.h"
#include "Fish.h"
#include "Skill.h"
#include "../Engine/Rapidjson.h"

class Dialog : public Component
{
public:

    Dialog();
    void LoadDialog(int id, double typingSpeed);
    void Update(double dt);
    void Draw();
    void Unload();


private:
    GLTexture* DialogBox = nullptr;
    GLTexture* Box_effect = nullptr;
    GLTexture* Me = nullptr;
    GLTexture* Sellerkeeper_1 = nullptr;
    GLTexture* Perry = nullptr;

    vec2 boxposition;
    mat3 back_matrix;
    mat3 effect_matrix;
    mat3 me_matrix;
    mat3 seller_martix;
    mat3 perry_martix;

    JsonParser_dialog* dialog;
    std::string fullText;
    std::string displayedText;
    std::string character;
    std::string idToString;
    size_t currentIndex;
    double elapsedTime;
    double typingSpeed;
    bool IsTyping;
};