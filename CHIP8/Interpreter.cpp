#include "Decode.h"
#include "Interpreter.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <print>

Interpreter::Interpreter(int cyclesPerSec, const std::string_view& romPath)
{
    // Set the cycles per second
    m_cyclesPerSec = cyclesPerSec;

    // Load the font into memory
    m_memory = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Load the ROM into memory
    std::ifstream rom(romPath.data(), std::ifstream::binary);
    size_t romSize{ std::filesystem::file_size(romPath) };
    if (romSize < m_memory.size() - m_pc)
        rom.read((char*)&m_memory[m_pc], romSize); // TODO : should replace this with a static cast
}

void Interpreter::Run()
{
    while (m_platform.GetIsOpen())
    {
        if (m_cyclesPerSec == MaxCycles || ShouldCycle())
        {
            Cycle();
            TickTimers();
        }
        m_platform.PollEvents(m_keypad);
    }
}

void Interpreter::Cycle()
{
    // Fetch and decode the instruction
    Instruction ins((m_memory[m_pc++] << 8) | m_memory[m_pc++]);
    // Exectute the instruction
    switch (ins.Opcode)
    {
    case Op::Cls00E0:
        m_display = {};
        m_platform.DrawDisplay(m_display);
        break;
    case Op::Ret00EE:
        m_pc = m_stack[--m_sp];
        break;
    case Op::Jp1nnn:
        m_pc = ins.NNN;
        break;
    case Op::Call2nnn:
        m_stack[m_sp++] = m_pc;
        m_pc = ins.NNN;
        break;
    case Op::Se3xnn:
        m_pc += m_v[ins.X] == ins.NN ? 2 : 0;
        break;
    case Op::Sne4xnn:
        m_pc += m_v[ins.X] != ins.NN ? 2 : 0;
        break;
    case Op::Se5xy0:
        m_pc += m_v[ins.X] == m_v[ins.Y] ? 2 : 0;
        break;
    case Op::Ld6xnn:
        m_v[ins.X] = ins.NN;
        break;
    case Op::Add7xnn:
        m_v[ins.X] += ins.NN;
        break;
    case Op::Ld8xy0:
        m_v[ins.X] = m_v[ins.Y];
        break;
    case Op::Or8xy1:
        m_v[ins.X] |= m_v[ins.Y];
        m_v[0xF] = 0;
        break;
    case Op::And8xy2:
        m_v[ins.X] &= m_v[ins.Y];
        m_v[0xF] = 0;
        break;
    case Op::Xor8xy3:
        m_v[ins.X] ^= m_v[ins.Y];
        m_v[0xF] = 0;
        break;
    case Op::Add8xy4:
        m_v[ins.X] += m_v[ins.Y];
        m_v[0xF] = m_v[ins.X] + m_v[ins.Y] > UINT8_MAX;
        break;
    case Op::Sub8xy5:
        m_v[ins.X] -= m_v[ins.Y];
        m_v[0xF] = m_v[ins.X] > m_v[ins.Y];
        break;
    case Op::Shr8xy6:
        m_v[0xF] = m_v[ins.X] & 1;
        m_v[ins.X] >>= 1;
        break;
    case Op::Subn8xy7:
        m_v[ins.X] = m_v[ins.Y] - m_v[ins.X];
        m_v[0xF] = m_v[ins.X] - m_v[ins.Y] < 0;
        break;
    case Op::Shl8xyE:
        m_v[0xF] = (m_v[ins.X] >> 7) & 1;
        m_v[ins.X] <<= 1;
    case Op::Sne9xy0:
        m_pc += m_v[ins.X] != m_v[ins.Y] ? 2 : 0;
        break;
    case Op::LdAnnn:
        m_i = ins.NNN;
        break;
    case Op::JpBnnn:
        m_pc = ins.NNN + m_v[0];
        break;
    case Op::RndCxnn:
        m_v[ins.X] = std::rand() % ins.NN; // TODO : swap this to a c++ rng
        break;
    case Op::DrwDxyn:
        for (int yOff{}; yOff < ins.N; yOff++)
            for (int xOff{}; xOff < 8; xOff++)
                if (m_memory[m_i + yOff] & (0x80 >> xOff))
                    SetPixel(m_v[ins.X] + xOff, m_v[ins.Y] + yOff);
        m_platform.DrawDisplay(m_display);
        break;
    case Op::SkpEx9E:
        m_pc += m_keypad[m_v[ins.X]] ? 2 : 0;
        break;
    case Op::SknpExA1:
        m_pc += !m_keypad[m_v[ins.X]] ? 2 : 0;
        break;
    case Op::LdFx07:
        m_v[ins.X] = m_delayTimer;
        break;
    case Op::LdFx0A:
        m_pc -= m_keypad[m_v[ins.X]] ? 0 : 2;
        break;
    case Op::LdFx15:
        m_delayTimer = m_v[ins.X];
        break;
    case Op::LdFx18:
        m_soundTimer = m_v[ins.X];
        break;
    case Op::AddFx1E:
        m_i += m_v[ins.X];
        break;
    case Op::LdFx29:
        m_i = m_v[ins.X] * 5;
        break;
    case Op::LdFx33:
        m_memory[m_i] = m_v[ins.X] / 100;
        m_memory[m_i + 1] = (m_v[ins.X] / 10) % 10;
        m_memory[m_i + 2] = (m_v[ins.X] % 100) % 10;
        break;
    case Op::LdFx55:
        for (int i{}; i <= ins.X; i++)
            m_memory[m_i++] = m_v[i];
        m_i++;
        break;
    case Op::LdFx65:
        for(int i{}; i <= ins.X; i++)
            m_v[i] = m_memory[m_i++];
        m_i++;
        break;
    }
}

void Interpreter::TickTimers()
{
    m_delayTimer -= m_delayTimer > 0;
    m_soundTimer -= m_soundTimer > 0;
}

void Interpreter::SetPixel(int x, int y)
{
    // Wrapping
    x %= 64;
    y %= 32;
    // Set the pixel and flag
    m_v[0xF] = m_display[y * 64 + x];
    m_display[y * 64 + x] = m_display[y * 64 + x] ? false : true;
}

bool Interpreter::ShouldCycle()
{
    constexpr int ms = 1000;
    static uint64_t prevCycle = GetTimeMs();
    uint64_t delta = GetTimeMs() - prevCycle;
    if (delta >= ms / m_cyclesPerSec)
    {
        prevCycle = GetTimeMs();
        return true;
    }
    return false;
}

uint64_t Interpreter::GetTimeMs()
{
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
    return ms;
}
