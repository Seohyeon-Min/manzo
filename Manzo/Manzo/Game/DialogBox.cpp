
#include "../Engine/Rapidjson.h"
#include "DialogBox.h"

Dialog::Dialog() 
	:currentIndex(0), elapsedTime(0.0),typingSpeed(0.05),IsTyping(false)
{
	 dialog = new CS230::JsonParser_dialog("en");
	 DialogBox = Engine::GetTextureManager().Load("assets/images/temp_dial.png");
	 boxposition = {50,100};
}

void Dialog::LoadDialog(int id, double Speed) {
	idToString = std::to_string(id);
	fullText = dialog->GetText(idToString);
	character = dialog->GetCharacter(idToString);
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

	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	back_matrix = mat3::build_translation({ 0, -(Engine::window_height)/3}) 
		* mat3::build_scale(1.f)
		* mat3::build_rotation(3.141592f / 1.0f);

	draw_call =
	{
		DialogBox,
		&back_matrix,
		Engine::GetShaderManager().GetDefaultShader(),
		nullptr,
		settings
	};
	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);

	if (displayedText != "") {
		Engine::GetFontManager().PrintText(FontType::Thin, displayedText.c_str(), {-0.5f,-0.62f}, 0.0f, 0.00013f, {1.0f,1.0f,1.0f});
		Engine::GetFontManager().PrintText(FontType::Bold, character.c_str(), { -0.5f,-0.52f }, 0.0f, 0.0001f, { 1.0f,1.0f,1.0f });		
	}
}

void Dialog::Unload() {
	displayedText = "";
}