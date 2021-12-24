#pragma once

#include <cstdint>
#include <array>

/**
 * 16-Bit General Purpose Registers (not including SP and PC)
 */
enum class Register16
{
    AF,
    BC,
    DE,
    HL,
    SP,
    PC
};

/**
 * 8-Bit General Purpose Registers
 */
enum class Register8
{
    A, F, B, C, D, E, H, L
};

class RegisterSet
{
public:
    using R8 = Register8;
    using R16 = Register16;
    RegisterSet(uint16_t _AF, uint16_t _BC, uint16_t _DE, uint16_t _HL, uint16_t _SP, uint16_t _PC);
    /**
     * Retrieves the 8-Bit value inside register @rg
     */
    uint8_t r(Register8 rg) const;
    /**
     * Sets the 8-Bit value @_v into register @rg
     */
    void r(Register8 rg, uint8_t _v);
    /**
     * Retrieves the 16-Bit value inside register @rg
     */
    uint16_t r(Register16 rg) const;
    /**
     * Sets the 16-Bit value @_v into register @rg
     */
    void r(Register16 rg, uint16_t _v);
public:
    bool Z = false, N = false, H = false, C = false;
private:
    std::array<uint8_t, 8> r8bit;
    uint16_t SP;
    uint16_t PC;
};
