#pragma once
#include <array>

struct DrawSettings {
    bool                  modulate_color = false;
    bool                  do_blending = false;
    bool                  is_camera_fixed = false;
    bool                  glow = false;
};