#pragma once

class WindowState {
public:
    static float GetWorldWidth() { return world_width; }
    static float GetWorldHeight() { return world_height; }
    static bool GetIsFullScreen() { return is_full_screen; }

    static void SetIsFullScreen(bool full) { is_full_screen = full; }
    static void SetWorldSize(float width, float height) {
        world_width = width;
        world_height = height;
    }

private:
    static bool is_full_screen;
    static float world_width;
    static float world_height;
};