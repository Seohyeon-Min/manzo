#define SDL_MAIN_HANDLED 
#include <iostream>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"
#include "Game/Mode2.h"

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    return  Engine::Instance();
}

int main() {
    try {
        GLApp OpenGLAPPlication("Manzo");
        Engine* engine = Engine::Instance();
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

        //engine->GetFontManager().AddFontType("assets/fonts/Font1.ttf");
        //engine->GetFontManager().AddFontType("assets/fonts/Font2.ttf");

        Mode1 mode1;
        engine->GetGameStateManager().AddGameState(mode1);

        Mode2 mode2;
        engine->GetGameStateManager().AddGameState(mode2);

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