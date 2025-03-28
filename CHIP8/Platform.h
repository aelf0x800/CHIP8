#pragma once

#include <bitset>
#include <optional>
#include <SDL3/SDL.h>

class Platform
{
private:
    static constexpr int s_width = 64;
    static constexpr int s_height = 32;
    static constexpr int s_scale = 10;

    SDL_Window *m_window{};
    SDL_Renderer *m_renderer{};
    
public:
    Platform();
    ~Platform();

    void DrawDisplay(const std::bitset<s_width * s_height>& display);
    bool PollEvent();
    void UpdateKeypad(std::bitset<16>& keypad);

private:
    std::optional<int> KeyToKeypadIndex(SDL_Keycode key);
};
