#pragma once

#include "gbcpu.h"
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace Opcodes
{
    class Opcode
    {
    public:
        virtual void execute(class GbCpu& cpu) = 0;
    };

    class Nop : public Opcode
    {
    public:
        static constexpr auto description = "No operation";
        static constexpr auto mnemonic = "NOP";
        static constexpr auto cycles = 4;
        static constexpr auto opcode = 0x00;
        static constexpr auto length = 1; // Length including the opcode
        static Nop parse(const std::vector<uint8_t>& bytes);
    public:
        virtual void execute(GbCpu& cpu) override;
    };

    class Jump : public Opcode
    {
    public:
        static constexpr auto description = "Jump to address nn";
        static constexpr auto mnemonic = "JP 0x{:0>4x}";
        static constexpr auto cycles = 12;
        static constexpr auto opcode = 0xC3;
        static constexpr auto length = 3;
        static Jump parse(const std::vector<uint8_t>& bytes);
    public:
        virtual void execute(GbCpu& cpu) override;
    public:
        uint16_t address;
    };
}

std::string to_string_debug(Opcodes::Nop nop);
std::string to_string_debug(Opcodes::Jump jump);

