#define SDL_MAIN_HANDLED 
#include <iostream>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"
#include "Game/Mode2.h"
#include "Game/TutorialMap.h"
#include "Game/Title.h"

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    return  Engine::Instance();
}

int main() {
    try {
        GLApp OpenGLAPPlication("Manzo");
        Engine* engine = Engine::Instance();

        engine->GetFontManager().AddFontType("assets/fonts/Font1.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/Font2.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/AlumniSans-Medium.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/AlumniSans-Light.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/Font4.ttf");

        std::ofstream saveFile("assets/scenes/save_data.txt", std::ios::trunc);

        Title title;
        engine->GetGameStateManager().AddGameState(title);

        Mode1 mode1;
        engine->GetGameStateManager().AddGameState(mode1);

        Mode2 mode2;
        engine->GetGameStateManager().AddGameState(mode2);

        Tutorial tutorial;
        engine->GetGameStateManager().AddGameState(tutorial);



        SDL_GL_SetSwapInterval(1);

        while (!OpenGLAPPlication.IsDone() && !engine->HasGameEnded()) {
            OpenGLAPPlication.Update();
        }

        engine->Stop();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}