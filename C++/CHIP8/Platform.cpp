#include "Interpreter.h"
#include "Platform.h"
#include <print>

Platform::Platform()
{
    // Initialise the SDL context
    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow("CHIP-8", s_width * s_scale, s_height * s_scale, 0);
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    SDL_SetRenderScale(m_renderer, s_scale, s_scale);
}

Platform::~Platform()
{
    // Free the SDL context
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Platform::DrawDisplay(const std::bitset<s_width * s_height>& display)
{
    // Draw the monochrome display pixel by pixel
    for (int y{}; y < s_height; y++)
    {
        for (int x{}; x < s_width; x++)
        {
            if (display[y * s_width + x])
                SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            else
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderPoint(m_renderer, x, y);
        }
    }
    SDL_RenderPresent(m_renderer);
}

bool Platform::PollEvent()
{
    // Poll for an event then return if the window is still open
    while (SDL_PollEvent(&m_event))
        if (m_event.type == SDL_EVENT_QUIT)
            return false;

    return true;
}

void Platform::UpdateKeypad(std::bitset<16>& keypad)
{
    if (m_event.type == SDL_EVENT_KEY_DOWN)
    {
        auto index = KeyToKeypadIndex(m_event.key.key);
        if (index.has_value())
            keypad[index.value()] = true;
    }
    else if (m_event.type == SDL_EVENT_KEY_UP)
    {
        auto index = KeyToKeypadIndex(m_event.key.key);
        if (index.has_value())
            keypad[index.value()] = false;
    }
}

std::optional<int> Platform::KeyToKeypadIndex(SDL_Keycode key)
{
    // Convert the SDL key to an index for the CHIP-8 keypad
    switch (key)
    {
    case SDLK_1:
        return 0x1;
    case SDLK_2:
        return 0x2;
    case SDLK_3:
        return 0x3;
    case SDLK_4:
        return 0xC;
    case SDLK_Q:
        return 0x4;
    case SDLK_W:
        return 0x5;
    case SDLK_E:
        return 0x6;
    case SDLK_R:
        return 0xD;
    case SDLK_A:
        return 0x7;
    case SDLK_S:
        return 0x8;
    case SDLK_D:
        return 0x9;
    case SDLK_F:
        return 0xE;
    case SDLK_Z:
        return 0xA;
    case SDLK_X:
        return 0x0;
    case SDLK_C:
        return 0xB;
    case SDLK_V:
        return 0xF;
    default:
        return std::nullopt;
    }
}
