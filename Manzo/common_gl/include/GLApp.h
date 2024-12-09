/*
 * Rudy Castan
 * CS200
 * Fall 2022
 */

#pragma once

#include "owner.h"

struct SDL_Window;
typedef void* SDL_GLContext;
class IProgram;

class [[nodiscard]] GLApp
{
public:
    GLApp(const char* title = "Manzo", int desired_width = 1280, int desired_height = 720);
    ~GLApp();

    GLApp(const GLApp&) = delete;
    GLApp& operator=(const GLApp&) = delete;
    GLApp(GLApp&&) noexcept = delete;
    GLApp& operator=(GLApp&&) noexcept = delete;

    void Update();
    bool IsDone() const noexcept;
    //void AdjustProjection(int window_width, int window_height, float base_width, float base_height);

    // 전체화면 전환/해제 함수 추가
    void ToggleFullscreen(bool enable);

private:
    util::owner<IProgram*>     ptr_program = nullptr;
    util::owner<SDL_Window*>   ptr_window = nullptr;
    util::owner<SDL_GLContext> gl_context = nullptr;
    bool                       is_done = false;
    float                      base_width, base_height;
};
