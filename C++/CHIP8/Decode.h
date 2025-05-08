#pragma once

#include <cstdint>

enum struct Op
{
    None, Cls00E0, Ret00EE, Jp1nnn, Call2nnn, Se3xnn, Sne4xnn, Se5xy0, Ld6xnn, Add7xnn, Ld8xy0,
    Or8xy1, And8xy2, Xor8xy3, Add8xy4, Sub8xy5, Shr8xy6, Subn8xy7, Shl8xyE, Sne9xy0, LdAnnn,
    JpBnnn, RndCxnn, DrwDxyn, SkpEx9E, SknpExA1, LdFx07, LdFx0A, LdFx15, LdFx18, AddFx1E, LdFx29,
    LdFx33, LdFx55, LdFx65
};

struct Instruction
{
    Op Opcode;
    uint8_t X;
    uint8_t Y;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;

    Instruction(uint16_t raw);
};
