#include "Engine/Engine.h"
#include "Game/Sea.h"

#include <iostream>

int main() {
    try {
        Engine& engine = Engine::Instance();
        engine.Start("Fishes appear randomly");

        Sea sea;
        engine.GetGameStateManager().AddGameState(sea);

        while (engine.HasGameEnded() == false) {
            engine.Update();
        }

        engine.Stop();

        return 0;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}
