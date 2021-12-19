#include "gbcpu.h"
#include "gbmemory.h"
#include "opcodes.h"
#include "bit_utils.h"
#include <iostream>
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/color.h>

GbCpu::GbCpu()
{
    inst_bytes.reserve(8);
    reg(Register16::AF, 0x0001);
    reg(Register16::BC, 0x0013);
    reg(Register16::DE, 0x00D8);
    reg(Register16::HL, 0x014D);
    memory[0xFF05] = 0x00; // TIMA
    memory[0xFF06] = 0x00; // TMA
    memory[0xFF07] = 0x00; // TAC
    memory[0xFF10] = 0x80; // NR10
    memory[0xFF11] = 0xBF; // NR11
    memory[0xFF12] = 0xF3; // NR12
    memory[0xFF14] = 0xBF; // NR14
    memory[0xFF16] = 0x3F; // NR21
    memory[0xFF17] = 0x00; // NR22
    memory[0xFF19] = 0xBF; // NR24
    memory[0xFF1A] = 0x7F; // NR30
    memory[0xFF1B] = 0xFF; // NR31
    memory[0xFF1C] = 0x9F; // NR32
    memory[0xFF1E] = 0xBF; // NR33
    memory[0xFF20] = 0xFF; // NR41
    memory[0xFF21] = 0x00; // NR42
    memory[0xFF22] = 0x00; // NR43
    memory[0xFF23] = 0xBF; // NR30
    memory[0xFF24] = 0x77; // NR50
    memory[0xFF25] = 0xF3; // NR51
    memory[0xFF26] = 0xF1; // NR52
    memory[0xFF40] = 0x91; // LCDC
    memory[0xFF42] = 0x00; // SCY
    memory[0xFF43] = 0x00; // SCX
    memory[0xFF45] = 0x00; // LYC
    memory[0xFF47] = 0xFC; // BGP
    memory[0xFF48] = 0xFF; // OBP0
    memory[0xFF49] = 0xFF; // OBP1
    memory[0xFF4A] = 0x00; // WY
    memory[0xFF4B] = 0x00; // WX
    memory[0xFFFF] = 0x00; // IE
}

void GbCpu::cycle()
{

    // const auto start = PC++;
    inst_bytes.clear();
    auto address = PC;

    inst_bytes.push_back(memory.read_byte(PC++));

    InstructionTrait flags = InstructionTrait::none;
    R8 changed_8bit_reg = R8::A;
    uint8_t previous_8bit_reg_value;
    R16 changed_16bit_reg = R16::AF;
    uint16_t previous_16bit_reg_value;
    // bool pZ = Z, pN = N, pH = H, pC = C;
    bool jumped = false;


    switch(inst_bytes[0])
    {
    case Opcodes::Nop::opcode:
    {
        auto nop = Opcodes::Nop{};
        std::cout << to_string_debug(nop) << std::endl;
        break;
    }
    case Opcodes::Jump::opcode:
    {
        inst_bytes.push_back(memory.read_byte(PC++));
        inst_bytes.push_back(memory.read_byte(PC++));
        auto jmp = Opcodes::Jump::parse(inst_bytes);
        std::cout << to_string_debug(jmp) << std::endl;
        jmp.execute(*this);
        break;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    ////// 16-Bit Loads
    ///////////////////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------------------
    //---- LD r, nn
    // Loads immediate values into register r
    //---------------------------------------------------------------------------------

    case 0x01:
    {
        // cycles 12
        // LD BC, 16imm
        inst_bytes.push_back(memory.read_byte(PC++));
        inst_bytes.push_back(memory.read_byte(PC++));
        auto imm = order_bytes(inst_bytes.data() + 1);

        reg(R16::BC, imm);

        changed_16bit_reg = R16::BC;

        break;
    }

    case 0x11:
    {
        // cycles 12
        // LD DE, 16imm
        break;
    }

    case 0x21:
    {
        // cycles 12
        // LD HL, 16imm
        break;
    }

    case 0x31:
    {
        // cycles 12
        // LD SP, 16imm
        break;
    }

    case 0x3E:
    {
        // cycles 8
        // LD A, #
        previous_8bit_reg_value = reg(Register8::A);
        flags = InstructionTrait::modifies_8bit_register;
        changed_8bit_reg = Register8::A;

        uint8_t n = memory.read_byte(PC++);
        inst_bytes.push_back(n);
        reg(Register8::A) = n;
        std::cout << fmt::format("LD A, {:0<2x}", static_cast<int>(n)) << std::endl;
        break;
    }
    case 0xF3:
    {
        // cycles 4
        // DI
        disable_interrupts_at = PC + 1;
        std::cout << "DI" << std::endl;
        break;
    }
    case 0xE0:
    {
        // cycles 12
        // LDH (n), A
        auto imm = memory.read_byte(PC++);
        inst_bytes.push_back(imm);
        memory[0xFF00+imm] = reg(Register8::A);
        std::cout << fmt::format("LD ({:0<4x}), A", 0xFF00+imm) << std::endl;
        flags = InstructionTrait::writes_to_memory;
        break;
    }
    case 0xF0:
    {
        // cycles 12
        // LDH A, (n)
        auto imm = memory.read_byte(PC++);
        inst_bytes.push_back(imm);
        reg(Register8::A) = memory[0xFF00+imm];
        std::cout << fmt::format("LD A, ({:0<4x})", 0xFF00+imm) << std::endl;
        flags = InstructionTrait::modifies_8bit_register;
        break;
    
    }
    case 0xAF:
    {
        // cycles 4
        // XOR A
        reg(Register8::A) = reg(Register8::A) ^ reg(Register8::A);
        std::cout << "XOR A" << std::endl;
        flags = InstructionTrait::modifies_8bit_register;
        break;
    }
    case 0xFE:
    {
        // cycles 8
        // CP n
        uint8_t imm = memory.read_byte(PC++);
        inst_bytes.push_back(imm);
        auto result = reg(Register8::A) - imm;
        Z = result == 0;
        N = true;
        H = ((0x0F & reg(Register8::A)) - (0x0F & imm)) < 0;
        C = result < 0;
        std::cout << fmt::format("CP {} ; A({}) - {}", imm, reg(R8::A), imm) << std::endl;
        flags = InstructionTrait::modifies_flags;
        break;
    }
    case 0x20:
    {
        // cycles 8
        // J n
        uint8_t imm = memory.read_byte(PC++);
        inst_bytes.push_back(imm);
        auto result = reg(Register8::A) - imm;
        Z = result == 0;
        N = true;
        H = ((0x0F & reg(Register8::A)) - (0x0F & imm)) < 0;
        C = result < 0;
        std::cout << fmt::format("CP {} ; A({}) - {}", imm, reg(R8::A), imm) << std::endl;
        flags = InstructionTrait::modifies_flags;
        break;
    }
    default:
        throw std::runtime_error{ fmt::format("Unknown opcode: {:0<2x}", inst_bytes[0]) };
    }

    if (disable_interrupts_at == PC)
    {
        std::cout << "Interrups disabled" << std::endl;
        disable_interrupts_at = 0x00;
    }

    fmt::print(fmt::fg(fmt::color::green), "0x{:0<4x} | ", address);
    for (auto b : inst_bytes)
    {
        fmt::print("{:0<2x} ", b);
    }
    fmt::print("\n");

    using namespace magic_enum::bitwise_operators;
    if ( (flags & InstructionTrait::modifies_8bit_register) == InstructionTrait::modifies_8bit_register )
    {
        fmt::print(fmt::fg(fmt::color::red),
                   "{}: {:0<2x} -> {:0<2x}\n",
                   magic_enum::enum_name(changed_8bit_reg),
                   previous_8bit_reg_value,
                   reg(changed_8bit_reg));
    }

    if ( (flags & InstructionTrait::modifies_16bit_register) == InstructionTrait::modifies_16bit_register )
    {
        fmt::print(fmt::fg(fmt::color::blue),
                   "{}: {:0<4x} -> {:0<4x}\n",
                   magic_enum::enum_name(changed_16bit_reg),
                   previous_16bit_reg_value,
                   reg(changed_16bit_reg));
    }

    if ( (flags & InstructionTrait::modifies_flags) == InstructionTrait::modifies_flags )
    {
        fmt::print(fmt::fg(fmt::color::burly_wood),
                   "Z {}, N {}, C {}, H {}\n",
                   Z ? "set" : "unset",
                   N ? "set" : "unset",
                   C ? "set" : "unset",
                   H ? "set" : "unset");
    }
}

void GbCpu::jump(uint16_t address)
{
    PC = address;
}

GbMemory& GbCpu::get_memory()
{
    return memory;
}
