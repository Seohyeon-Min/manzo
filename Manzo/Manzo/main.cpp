#define SDL_MAIN_HANDLED 
#include <iostream>
#include <ctime>

#include "Engine/Engine.h"
#include "Game/Splash.h"
#include "Game/Mode1.h"
#include "Game/Mode2.h"
#include "Game/Loading.h"
#include "Game/TutorialMap.h"
#include "Game/Title.h"
#include "Game/Mode3.h"
#include "Game/Splash.h"

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    glCheck(glViewport(0, 0, viewport_width, viewport_height));
    return  Engine::Instance();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    try {
        GLApp OpenGLAPPlication("Manzo");
        Engine* engine = Engine::Instance();

        engine->GetFontManager().AddFontType("assets/fonts/Font1.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/Font2.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/AlumniSans-Medium.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/AlumniSans-Light.ttf");
        engine->GetFontManager().AddFontType("assets/fonts/Font4.ttf");

        //shader
        Engine::GetShaderManager().LoadShader("water_ripple", "assets/shaders/default.vert", "assets/shaders/water_ripple.frag");
        Engine::GetShaderManager().LoadShader("icon", "assets/shaders/default.vert", "assets/shaders/edge_detection.frag");
        Engine::GetShaderManager().LoadShader("purple", "assets/shaders/default.vert", "assets/shaders/default_purple.frag");
        Engine::GetShaderManager().LoadShader("pixelate", "assets/shaders/default.vert", "assets/shaders/pixelate.frag");
        Engine::GetShaderManager().LoadShader("blur", "assets/shaders/default.vert", "assets/shaders/blur.frag");
        Engine::GetShaderManager().LoadShader("change_alpha", "assets/shaders/default.vert", "assets/shaders/change_alpha.frag");
        Engine::GetShaderManager().LoadShader("change_color", "assets/shaders/default.vert", "assets/shaders/change_color.frag");
        Engine::GetShaderManager().LoadShader("change_alpha_no_texture", "assets/shaders/default.vert", "assets/shaders/change_alpha_no_texture.frag");
        Engine::GetShaderManager().LoadShader("health_bar", "assets/shaders/default.vert", "assets/shaders/health_bar.frag");
        Engine::GetShaderManager().LoadShader("sea_background", "assets/shaders/post_default.vert", "assets/shaders/sea_background.frag");
        Engine::GetShaderManager().LoadShader("wave", "assets/shaders/default.vert", "assets/shaders/wave.frag");
        Engine::GetShaderManager().LoadShader("bossEbullet", "assets/shaders/default.vert", "assets/shaders/bossEbullet.frag");
        Engine::GetShaderManager().LoadShader("emissive", "assets/shaders/default.vert", "assets/shaders/emissive.frag");
        Engine::GetShaderManager().LoadShader("fade_in", "assets/shaders/default.vert", "assets/shaders/fade_in.frag");
        Engine::GetShaderManager().LoadShader("fade_out", "assets/shaders/default.vert", "assets/shaders/fade_out.frag");
        Engine::GetShaderManager().LoadShader("ink_transition", "assets/shaders/default.vert", "assets/shaders/ink_transition.frag");
        Engine::GetShaderManager().LoadShader("circle_pattern", "assets/shaders/default.vert", "assets/shaders/boss_E_circle_pattern.frag");
        Engine::GetShaderManager().LoadShader("light", "assets/shaders/light.vert", "assets/shaders/light.frag");

        Engine::GetShaderManager().LoadShader("under_water_god_ray", "assets/shaders/post_default.vert", "assets/shaders/underwater_god_ray.frag");
        Engine::GetShaderManager().LoadShader("post_default", "assets/shaders/post_default.vert", "assets/shaders/post_default.frag");
        Engine::GetShaderManager().LoadShader("post_bloom", "assets/shaders/post_default.vert", "assets/shaders/post_bloom.frag");
        Engine::GetShaderManager().LoadShader("post_underwater_distortion", "assets/shaders/post_default.vert", "assets/shaders/post_underwater_distortion.frag");

        Engine::GetShaderManager().LoadShader("under_water_god_ray", "assets/shaders/post_default.vert", "assets/shaders/underwater_god_ray.frag");
        Engine::GetShaderManager().LoadShader("post_default", "assets/shaders/post_default.vert", "assets/shaders/post_default.frag");
        Engine::GetShaderManager().LoadShader("post_bloom", "assets/shaders/post_default.vert", "assets/shaders/post_bloom.frag");
        Engine::GetShaderManager().LoadShader("post_underwater_distortion", "assets/shaders/post_default.vert", "assets/shaders/post_underwater_distortion.frag");
        Engine::GetShaderManager().LoadShader("title_ripple", "assets/shaders/post_default.vert", "assets/shaders/title_water_ripple.frag");
        Engine::GetShaderManager().LoadShader("title_gradation", "assets/shaders/post_default.vert", "assets/shaders/title_color_gradation.frag");
        Engine::GetShaderManager().LoadShader("image_distortion", "assets/shaders/default.vert", "assets/shaders/image_distortion.frag");
        Engine::GetShaderManager().LoadShader("post_water_wave", "assets/shaders/post_default.vert", "assets/shaders/post_water_wave.frag");
        Engine::GetShaderManager().LoadShader("post_wave_transition", "assets/shaders/post_default.vert", "assets/shaders/post_wave_transition.frag");

        Splash splash;
        engine->GetGameStateManager().AddGameState(splash);

        Title title;
        engine->GetGameStateManager().AddGameState(title);
        std::ofstream saveFile("assets/scenes/save_data.txt", std::ios::trunc);
        

        /*Loading loading;
        engine->GetGameStateManager().AddGameState(loading);*/

        Mode1 mode1;
        engine->GetGameStateManager().AddGameState(mode1);

        Mode2 mode2;
        engine->GetGameStateManager().AddGameState(mode2);

        Mode3 mode3;
        engine->GetGameStateManager().AddGameState(mode3);

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