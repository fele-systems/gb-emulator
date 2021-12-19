#include "opcodes.h"
#include "gbcpu.h"
#include "assembly.h"
#include "bit_utils.h"
#include <iostream>
#include <cassert>
#include <fmt/core.h>
#include "gbcpu.h"

Opcodes::Nop Opcodes::Nop::parse(const std::vector<uint8_t>& bytes)
{
    assert(bytes.size() == length);
    assert(bytes[0] == opcode);
    
    return Nop{};
}

void Opcodes::Nop::execute(GbCpu& cpu)
{
    return;
}

Opcodes::Jump Opcodes::Jump::parse(const std::vector<uint8_t>& bytes)
{
    assert(bytes.size() == length);
    assert(bytes[0] == opcode);

    Jump jmp;
    jmp.address = order_bytes(bytes.data() + 1);

    return jmp;
}

void Opcodes::Jump::execute(GbCpu& cpu)
{
    cpu.jump(address);
}

std::string to_string_debug(Opcodes::Nop nop)
{
    return "NOP";
};

std::string to_string_debug(Opcodes::Jump jmp)
{
    return fmt::format(Opcodes::Jump::mnemonic, jmp.address);
}
