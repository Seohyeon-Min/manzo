
#ifndef DIALOG_H
#define DIALOG_H

#include "../Engine/GameObject.h"
#include "../Engine/Rapidjson.h"
#include "../Engine/TextureManager.h"
#include "../Engine/FontManager.h"
#include "../Engine/Render.h"
#include "../Engine/ShaderManager.h"
#include <string>
#include <vector>

class Dialog : public GameObject {
public:
    Dialog(vec2 start_position);

    //void LoadDialog(int id, double Speed);
    void LoadDialogGroup(const std::string& group_id, double Speed = dialogBasicSpeed);
    void LoadRandomDialog(const std::string& group_id, double Speed = dialogBasicSpeed);
    void NextLine();
    void Update(double dt) override;
    void Draw();
    void Hide();
    void Unload();
    bool IsFinished() const;

    GameObjectTypes Type() override { return GameObjectTypes::Dialog; }
    std::string TypeName() override { return "Dialog"; }

private:
    void SetDrawSetting(DrawCall& draw_call);
    void StartLine(int index);

    JsonParser_dialog* dialog;
    std::string fullText;
    std::string displayedText;
    std::string character;
    std::string idToString;

    double elapsedTime;
    double typingSpeed;
    bool IsTyping;
    int currentIndex;
    bool isVisible = false;
    static constexpr double dialogBasicSpeed = 0.05;

    std::vector<std::pair<std::string, std::string>> currentDialogGroup;
    int currentLineIndex = 0;

    GLTexture* DialogBox;
    GLTexture* Box_effect;
    GLTexture* Me;
    GLTexture* Sellerkeeper_1;
    GLTexture* Perry;

    mat3 back_matrix;
    mat3 effect_matrix;
    mat3 me_matrix;
    mat3 seller_martix;
    mat3 perry_martix;

    vec2 boxposition;

    bool is_finish = false;
};

#endif
