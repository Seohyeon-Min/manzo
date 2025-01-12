
#include "../Engine/Rapidjson.h"
#include "DialogBox.h"

Dialog::Dialog() 
	:currentIndex(0), elapsedTime(0.0),typingSpeed(0.05),IsTyping(false)
{
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

	 me_matrix = mat3::build_translation({ ((Engine::window_width) / 3)+60, (-(Engine::window_height) / 3) + 50 })
		 * mat3::build_scale(0.2f)
		 * mat3::build_rotation(0.0f);

	 seller_martix = mat3::build_translation({ -((Engine::window_width) / 3) - 60, (-(Engine::window_height) / 3) + 50 })
		 * mat3::build_scale(0.2f)
		 * mat3::build_rotation(0.0f);

	 perry_martix = mat3::build_translation({ -((Engine::window_width) / 3) - 60, (-(Engine::window_height) / 3) + 50 })
		 * mat3::build_scale(0.2f)
		 * mat3::build_rotation(0.0f);

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


}

void Dialog::Unload() {
	//displayedText = "";
	DialogBox = nullptr;
	Box_effect = nullptr;
	Me = nullptr;
	Sellerkeeper_1 = nullptr;
	Perry = nullptr;
	dialog = nullptr;
}