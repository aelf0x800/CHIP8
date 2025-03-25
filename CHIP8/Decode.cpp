#include "Decode.h"

Instruction::Instruction(uint16_t raw)
{
    X = (raw & 0x0F00) >> 8;
    Y = (raw & 0x00F0) >> 4;
    NNN = raw & 0x0FFF;
    NN = raw & 0x00FF;
    N = raw & 0x000F;

    switch (raw & 0xF000)
    {
    case 0x0000:
        if (NN == 0xE0)
            Opcode = Op::Cls00E0;
        else if (NN == 0xEE)
            Opcode = Op::Ret00EE;
        else
            Opcode = Op::None;
        break;
    case 0x1000:
        Opcode = Op::Jp1nnn;
        break;
    case 0x2000:
        Opcode = Op::Call2nnn;
        break;
    case 0x3000:
        Opcode = Op::Se3xnn;
        break;
    case 0x4000:
        Opcode = Op::Sne4xnn;
        break;
    case 0x5000:
        Opcode = Op::Se5xy0;
        break;
    case 0x6000:
        Opcode = Op::Ld6xnn;
        break;
    case 0x7000:
        Opcode = Op::Add7xnn;
        break;
    case 0x8000:
        switch (N)
        {
        case 0x0:
            Opcode = Op::Ld8xy0;
            break;
        case 0x1:
            Opcode = Op::Or8xy1;
            break;
        case 0x2:
            Opcode = Op::And8xy2;
            break;
        case 0x3:
            Opcode = Op::Xor8xy3;
            break;
        case 0x4:
            Opcode = Op::Add8xy4;
            break;
        case 0x5:
            Opcode = Op::Sub8xy5;
            break;
        case 0x6:
            Opcode = Op::Shr8xy6;
            break;
        case 0x07:
            Opcode = Op::Subn8xy7;
            break;
        case 0xE:
            Opcode = Op::Shl8xyE;
            break;
        default:
            Opcode = Op::None;
            break;
        }
        break;
    case 0x9000:
        Opcode = Op::Sne9xy0;
        break;
    case 0xA000:
        Opcode = Op::LdAnnn;
        break;
    case 0xB000:
        Opcode = Op::JpBnnn;
        break;
    case 0xC000:
        Opcode = Op::RndCxnn;
        break;
    case 0xD000:
        Opcode = Op::DrwDxyn;
        break;
    case 0xE000:
        if (NN == 0x9E)
            Opcode = Op::SkpEx9E;
        else if (NN == 0xA1)
            Opcode = Op::SknpExA1;
        else
            Opcode = Op::None;
        break;
    case 0xF000:
        switch (NN)
        {
        case 0x07:
            Opcode = Op::LdFx07;
            break;
        case 0x0A:
            Opcode = Op::LdFx0A;
            break;
        case 0x15:
            Opcode = Op::LdFx15;
            break;
        case 0x18:
            Opcode = Op::LdFx18;
            break;
        case 0x1E:
            Opcode = Op::AddFx1E;
            break;
        case 0x29:
            Opcode = Op::LdFx29;
            break;
        case 0x33:
            Opcode = Op::LdFx33;
            break;
        case 0x55:
            Opcode = Op::LdFx55;
            break;
        case 0x65:
            Opcode = Op::LdFx65;
            break;
        default:
            Opcode = Op::None;
            break;
        }
        break;
    default:
        Opcode = Op::None;
        break;
    }
}
