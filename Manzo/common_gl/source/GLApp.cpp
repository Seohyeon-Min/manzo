/*
 * Rudy Castan
 * CS200
 * Fall 2022
 */

#include "GLApp.h"
#include "IProgram.h"
#include "ImGuiHelper.h"
#include "WindowState.h"
#include "mat3.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <SDL2/SDL_mouse.h>

#include <chrono> 
#include <thread>

namespace
{
    void hint_gl(SDL_GLattr attr, int value)
    {
        // https://wiki.libsdl.org/SDL_GL_SetAttribute
        if (const auto success = SDL_GL_SetAttribute(attr, value); success != 0)
        {
            std::cerr << "Failed to Set GL Attribute: " << SDL_GetError() << '\n';
        }
    }

    template <typename... Messages> [[noreturn]] void throw_error_message(Messages&&... more_messages)
    {
        std::ostringstream sout;
        (sout << ... << more_messages);
        throw std::runtime_error{ sout.str() };
    }
}


GLApp::GLApp(const char* title, int desired_width, int desired_height)
{
    base_width = (float)desired_width;
    base_height = (float)desired_height;
    WindowState::SetWorldSize(base_width, base_height);

    if (title == nullptr || title[0] == '\0')
        throw_error_message("App title shouldn't be empty");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw_error_message("Failed to init SDK error: ", SDL_GetError());
    }

    SDL_ShowCursor(SDL_FALSE);

    hint_gl(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    hint_gl(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    hint_gl(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    hint_gl(SDL_GL_DOUBLEBUFFER, true);
    hint_gl(SDL_GL_STENCIL_SIZE, 8);
    hint_gl(SDL_GL_DEPTH_SIZE, 24);
    hint_gl(SDL_GL_RED_SIZE, 8);
    hint_gl(SDL_GL_GREEN_SIZE, 8);
    hint_gl(SDL_GL_BLUE_SIZE, 8);
    hint_gl(SDL_GL_ALPHA_SIZE, 8);
    hint_gl(SDL_GL_MULTISAMPLEBUFFERS, 1);
    hint_gl(SDL_GL_MULTISAMPLESAMPLES, 4);

    desired_width = std::max(640, std::min(16384, desired_width));
    desired_height = std::max(480, std::min(16384, desired_height));
    ptr_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, desired_width, desired_height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (ptr_window == nullptr)
    {
        throw_error_message("Failed to create window: ", SDL_GetError());
    }

    if (gl_context = SDL_GL_CreateContext(ptr_window); gl_context == nullptr)
    {
        throw_error_message("Failed to create opengl context: ", SDL_GetError());
    }

    SDL_GL_MakeCurrent(ptr_window, gl_context);

    if (const auto result = glewInit(); GLEW_OK != result)
    {
        throw_error_message("Unable to initialize GLEW - error: ", glewGetErrorString(result));
    }

    constexpr int ADAPTIVE_VSYNC = -1;
    constexpr int VSYNC = 1;
    if (const auto result = SDL_GL_SetSwapInterval(ADAPTIVE_VSYNC); result != 0)
    {
        SDL_GL_SetSwapInterval(VSYNC);
    }

    ImGuiHelper::Initialize(ptr_window, gl_context);

    int width = 0, height = 0;
    SDL_GL_GetDrawableSize(ptr_window, &width, &height);
    ptr_program = create_program(width, height);
}


GLApp::~GLApp()
{
    delete ptr_program;
    ImGuiHelper::Shutdown();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(ptr_window);
    SDL_Quit();
}

void GLApp::Update() {
    using namespace std::chrono;

    static system_clock::time_point last_frame_time = system_clock::now();
    const double target_frame_time = 1.0 / 240;

    SDL_Event event{ 0 };
    static bool is_fullscreen = false;

    while (SDL_PollEvent(&event) != 0) {
        ImGuiHelper::FeedEvent(event);
        ptr_program->HandleEvent(*ptr_window, event);

        if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) [[unlikely]] {
            is_done = true;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT)) {
                    is_fullscreen = !is_fullscreen;
                    ToggleFullscreen(is_fullscreen);
                }
            }
    }

    ptr_program->Update();
#ifdef _DEBUG
    ImGuiHelper::Begin();
    ptr_program->ImGuiDraw();
    ImGuiHelper::End(ptr_window, gl_context);
#else
#endif

    SDL_GL_SwapWindow(ptr_window);

    system_clock::time_point now = system_clock::now();
    double frame_duration = duration<double>(now - last_frame_time).count();

    if (frame_duration < target_frame_time) {
        std::this_thread::sleep_for(duration<double>(target_frame_time - frame_duration));
    }

    last_frame_time = system_clock::now();
}

void GLApp::ToggleFullscreen(bool enable) {
    if (enable) {
        SDL_DisplayMode display_mode;
        if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0) {
            std::cerr << "Failed to get desktop display mode: " << SDL_GetError() << std::endl;
            return;
        }

        // 전체화면 전환
        if (SDL_SetWindowFullscreen(ptr_window, SDL_WINDOW_FULLSCREEN) != 0) {
            std::cerr << "Failed to set fullscreen mode: " << SDL_GetError() << std::endl;
        }

        // OpenGL 컨텍스트 다시 바인딩
        SDL_GL_MakeCurrent(ptr_window, gl_context);

        // 모니터 해상도에 맞춰서 프로젝션 설정
        //AdjustProjection(display_mode.w, display_mode.h, base_width, base_height);

    }
    else {
        // 창 모드 복귀
        if (SDL_SetWindowFullscreen(ptr_window, 0) != 0) {
            std::cerr << "Failed to exit fullscreen mode: " << SDL_GetError() << std::endl;
        }

        SDL_SetWindowSize(ptr_window, (int)base_width, (int)base_height);

        // OpenGL 컨텍스트 다시 바인딩
        SDL_GL_MakeCurrent(ptr_window, gl_context);

        // 기본 해상도로 프로젝션 재설정
        //AdjustProjection((int)base_width, (int)base_height, base_width, base_height);
    }
}


//void GLApp::AdjustProjection(int window_width, int window_height, float base_width, float base_height) {
//    if (SDL_GL_GetCurrentContext() != gl_context) {
//        std::cerr << "OpenGL context mismatch! Rebinding context." << std::endl;
//        SDL_GL_MakeCurrent(ptr_window, gl_context);
//    }
//
//    float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
//    float base_aspect_ratio = base_width / base_height;
//
//    float world_width = base_width;
//    float world_height = base_height;
//
//    // 원본 비율을 유지하면서 모니터 해상도에 맞춰 확대/축소
//    if (aspect_ratio > base_aspect_ratio) {
//        world_width = world_height * aspect_ratio;
//    }
//    else if (aspect_ratio < base_aspect_ratio) {
//        world_height = world_width / aspect_ratio;
//    }
//
//    std::cout << "Setting World Size (Fit to Monitor): " << world_width << "x" << world_height << std::endl;
//
//    glViewport(0, 0, window_width, window_height);
//
//    WindowState::SetWorldSize((float)window_width, (float) window_height);
//
//    mat3 projection = mat3(
//        2.0f / world_width, 0.0f, 0.0f,
//        0.0f, 2.0f / world_height, 0.0f,
//        -1.0f, -1.0f, 1.0f
//    );
//
//    // Engine을 통해 Render 인스턴스에 접근 가능하다고 가정
//    //Engine::GetRender().SetProjection(projection);
//}

bool GLApp::IsDone() const noexcept
{
    return is_done;
}
