#pragma once

#include "Platform.h"
#include <array>
#include <bitset>
#include <string_view>

class Interpreter
{
private:
    Platform m_platform;
    
    std::bitset<64 * 32> m_display{};
    std::bitset<16> m_keypad{};
    
    std::array<uint8_t, 2048> m_memory{};
    std::array<uint16_t, 12> m_stack{};
    
    uint16_t m_pc{ 0x200 };
    uint16_t m_i{};
    uint8_t m_sp{};
    std::array<uint8_t, 16> m_v{};
    
    uint8_t m_delayTimer{};
    uint8_t m_soundTimer{};

public:
    Interpreter(const std::string_view& romPath);
   
    void Run();

private:
    void Cycle();
    void TickTimers();
    void SetPixel(int x, int y);
};
