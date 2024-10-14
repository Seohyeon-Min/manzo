#define SDL_MAIN_HANDLED 
#include <iostream>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"
#include "Game/Mode2.h"

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    return &Engine::Instance();
}

int main() {
    try {
        GLApp OpenGLAPPlication("Manzo");
        Engine& engine = Engine::Instance();

        Splash splash;
        engine.GetGameStateManager().AddGameState(splash);

        Mode1 mode1;
        engine.GetGameStateManager().AddGameState(mode1);

        Mode2 mode2;
        engine.GetGameStateManager().AddGameState(mode2);

        SDL_GL_SetSwapInterval(1);

        while (!OpenGLAPPlication.IsDone() && !engine.HasGameEnded()) {
            engine.Update(); // Update the engine here
            OpenGLAPPlication.Update();
        }

        engine.Stop();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}