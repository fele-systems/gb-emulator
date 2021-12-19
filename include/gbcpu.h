#pragma once

#include "gbmemory.h"
#include "magic_enum.hpp"
#include <array>
#include <stdexcept>

enum class Register16
{
    AF,
    BC,
    DE,
    HL
};

enum class Register8
{
    A, F, B, C, D, E, H, L
};

enum class InstructionTrait
{
    none                        = 0,
    modifies_8bit_register      = 1 << 0,
    modifies_16bit_register     = 1 << 1,
    modifies_flags              = 1 << 2,
    writes_to_memory            = 1 << 3
};

class GbCpu
{
public:
    using R8 = Register8;
    using R16 = Register16;
    GbCpu();
    uint8_t& reg(Register8 num)
    {
        return r8bit[magic_enum::enum_integer(num)];
    }
    void reg(Register16 num, uint16_t value)
    {
        switch (num)
        {
            case Register16::AF:
                reg(Register8::F) = value & 0xFF;
                reg(Register8::A) = (value >> 8) & 0xFF;
                break;
            case Register16::BC:
                reg(Register8::C) = value & 0xFF;
                reg(Register8::B) = (value >> 8) & 0xFF;
                break;
            case Register16::DE:
                reg(Register8::E) = value & 0xFF;
                reg(Register8::D) = (value >> 8) & 0xFF;
                break;
            case Register16::HL:
                reg(Register8::L) = value & 0xFF;
                reg(Register8::H) = (value >> 8) & 0xFF;
                break;
            default:
                throw std::runtime_error{ "Invalid register" };
        }
    }
    uint16_t reg(Register16 num)
    {
        switch (num)
        {
            case Register16::AF:
                return (reg(Register8::A) << 8) | reg(Register8::F);
                break;
            case Register16::BC:
                return (reg(Register8::B) << 8) | reg(Register8::C);
                break;
            case Register16::DE:
                return (reg(Register8::D) << 8) | reg(Register8::E);
                break;
            case Register16::HL:
                return (reg(Register8::H) << 8) | reg(Register8::L);
                break;
            default:
                throw std::runtime_error{ "Invalid register" };
        }
    }
    void cycle();
    GbMemory& get_memory();
    void jump(uint16_t address);
private:

private:
    GbMemory memory;
    std::array<uint8_t, 8> r8bit;
    uint16_t SP = 0xFFFE;
    uint16_t PC = 0x0100;
    std::vector<uint8_t> inst_bytes;
    int disable_interrupts_at = 0;
    bool Z = false, N = false, H = false, C = false;
};
