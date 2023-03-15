#include "gbcpu.h"
#include "gbmemory.h"
#include "opcodes-autogen.h"
#include "bit_utils.h"
#include <iostream>
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/color.h>
#include <errno.h>
#include <magic_enum.hpp>

GbCpu::GbCpu()
{
    fetched_bytes.reserve ( 8 );
    
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

void GbCpu::push_word(uint16_t word)
{
    uint16_t sp = reg(R16::SP);
    memory.write_word(sp, word);
    sp += 2;
    reg(R16::SP, sp);
}


uint16_t GbCpu::pop_word()
{
    uint16_t sp = reg(R16::SP);
    sp -= 2;
    auto word = memory.read_word(sp);
    reg(R16::SP, sp);
    return word;
}

void GbCpu::cycle()
{

    // const auto start = PC++;
    fetched_bytes.clear();
    auto address = reg(R16::PC);

    auto registerSetCopy = registerSet;

    auto traits = execute_next();
    std::cout.flush();

    if ( disable_interrupts_at == reg(R16::PC) ) {
        std::cout << "Interrups disabled" << std::endl;
        disable_interrupts_at = 0x00;
    }

    if ( enable_interrupts_at == reg(R16::PC) ) {
        std::cout << "Interrupts enabled" << std::endl;
        enable_interrupts_at = 0x00;
    }

    fmt::print ( fmt::fg ( fmt::color::green ), "0x{:0>4x} | ", address );
    for ( auto b : fetched_bytes ) {
        fmt::print ( "{:0>2x} ", static_cast<int>(b) );
    }
    fmt::print ( "\n" );

    using namespace magic_enum::bitwise_operators;

    if ( traits.has(InstructionTrait::modifies_8bit_register) ) {
        fmt::print ( fmt::fg ( fmt::color::red ),
                     "{}: {:0>2x} -> {:0>2x}\n",
                     magic_enum::enum_name ( traits.get_modified_r8() ),
                     registerSetCopy.r( traits.get_modified_r8() ),
                     reg ( traits.get_modified_r8() ) );
    }

    if ( traits.has(InstructionTrait::modifies_16bit_register) ) {
        fmt::print ( fmt::fg ( fmt::color::red ),
                     "{}: {:0>4x} -> {:0>4x}\n",
                     magic_enum::enum_name ( traits.get_modified_r16() ),
                     registerSetCopy.r( traits.get_modified_r16() ),
                     reg ( traits.get_modified_r16() ) );
    }

    if ( traits.has(InstructionTrait::modifies_flags) ) {
        fmt::print ( fmt::fg ( fmt::color::burly_wood ),
                     "Z {}, N {}, C {}, H {}\n",
                     registerSet.Z ? "set" : "unset",
                     registerSet.N ? "set" : "unset",
                     registerSet.C ? "set" : "unset",
                     registerSet.H ? "set" : "unset" );
    }
}

GbMemory& GbCpu::get_memory()
{
    return memory;
}

InstructionTrait GbCpu::LD_r8_r8 (R8 r8out, R8 r8in)
{
    reg (r8out, reg(r8in));

    fmt::print ( "LD {}, {}\n", magic_enum::enum_name (r8out), magic_enum::enum_name (r8in) );

    return InstructionTrait{ r8out };
}

InstructionTrait GbCpu::LD_r8_pr8 (R8 r8, R8 pr8)
{
    uint16_t addr = 0xFF00 + reg(pr8);
    reg (r8, memory.read_byte(addr));

    fmt::print ( "LD {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(pr8), addr);

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_pr8_r8 (R8 pr8, R8 r8)
{
    uint16_t addr = 0xFF00 + reg(pr8);
    memory[addr] = reg(r8);

    fmt::print ( "LD ({}), {} = 0x{:0>4x}\n", magic_enum::enum_name(pr8), magic_enum::enum_name(r8), addr);

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_r8_pr16 (R8 r8, R16 pr16)
{
    uint16_t addr = reg(pr16);
    reg (r8, memory.read_byte(addr));

    fmt::print ( "LD {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(pr16), addr);

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_pr16_r8 (R16 pr16, R8 r8)
{
    uint16_t addr = reg(pr16);
    memory[addr] = reg(r8);

    fmt::print ( "LD ({}), {} = 0x{:0>4x}\n", magic_enum::enum_name(pr16), magic_enum::enum_name(r8), addr);

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_r8_8imm (R8 r8)
{
    uint8_t imm = next_pc_byte();
    reg ( r8, imm );

    fmt::print ( "LD {}, 0x{:0>2x}\n", magic_enum::enum_name ( r8 ), imm );

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_r16_16imm ( R16 r16 )
{
    uint16_t imm = next_pc_word();
    reg ( r16, imm );

    fmt::print ( "LD {}, 0x{:0>4x}\n", magic_enum::enum_name ( r16 ), imm );

    return InstructionTrait{ r16 };
}
InstructionTrait GbCpu::LD_r16_r16 (R16 r16out, R16 r16in)
{
    reg ( r16out, reg (r16in ) );

    fmt::print ( "LD {}, {}\n", magic_enum::enum_name ( r16out ), magic_enum::enum_name(r16in) );

    return InstructionTrait{ r16out };
}

InstructionTrait GbCpu::LDI_r8_p16r(GbCpu::R8 r8, GbCpu::R16 r16)
{
    uint16_t addr = reg (r16);
    reg ( r8, memory[addr] );
    reg ( r16, addr + 1 );

    fmt::print ( "LDI {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(r16), addr);

    return InstructionTrait{ r8 };
}


InstructionTrait GbCpu::LD_p16imm_r8(R8 r8)
{
    uint16_t imm = next_pc_word();
    memory[imm] = reg(r8);

    fmt::print ( "LD ({:0>4x}), {}\n", imm, magic_enum::enum_name(r8) );

    return InstructionTrait::no_traits();
}


InstructionTrait GbCpu::LD_r8_p16imm(R8 r8)
{
    uint16_t imm = next_pc_word();
    reg( r8, memory.read_byte(imm) );

    fmt::print ( "LD {}, ({:0>4x})\n", magic_enum::enum_name(r8), imm );

    return InstructionTrait{ r8 };
}

// InstructionTrait GbCpu::LD_r8_pr16(R8 r8, R16 r16)
// {
//     uint16_t addr = reg( r16 );
//     uint8_t byte = memory[addr];
//     reg ( r8, byte );

//     fmt::print ( "LD {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name (r8), magic_enum::enum_name (r16), addr );

//     return InstructionTrait{ r8 };
// }

// InstructionTrait GbCpu::LD_pr16_r8(R16 r16, R8 r8)
// {
//     uint16_t addr = reg( r16 );
//     memory[addr] = reg (r8);

//     fmt::print ( "LD ({}), {} = 0x{:0>4x}\n", magic_enum::enum_name (r16), magic_enum::enum_name (r8), addr );

//     return InstructionTrait{ r8 };
// }

InstructionTrait GbCpu::LD_pr16_8imm(R16 r16)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = reg ( r16 );

    memory[addr] = imm;

    fmt::print ( "LD ({}), 0x{:0>2x} = 0x{:0>4x}\n", magic_enum::enum_name (r16), imm, addr );

    return InstructionTrait::no_traits();
}

InstructionTrait GbCpu::LDD_r8_pr16(R8 r8, R16 pr16)
{
    uint16_t addr = reg (pr16);
    reg (r8, memory[addr]);
    reg (pr16, addr - 1);
    fmt::print ( "LDD {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name (r8), magic_enum::enum_name (pr16), addr );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LDD_pr16_r8(R16 pr16, R8 r8)
{
    uint16_t addr = reg (pr16);
    memory[addr] = reg (r8);
    reg (pr16, addr - 1);
    fmt::print ( "LDD ({}), {} = 0x{:0>4x}\n", magic_enum::enum_name (pr16), magic_enum::enum_name (r8), addr );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LDI_r8_pr16(R8 r8, R16 pr16)
{
    uint16_t addr = reg (pr16);
    reg (r8, memory[addr]);
    reg (pr16, addr + 1);
    fmt::print ( "LDI {}, ({}) = 0x{:0>4x}\n", magic_enum::enum_name (r8), magic_enum::enum_name (pr16), addr );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LDI_pr16_r8(R16 pr16, R8 r8)
{
    uint16_t addr = reg (pr16);
    memory[addr] = reg (r8);
    reg (pr16, addr + 1);
    fmt::print ( "LDI ({}), {} = 0x{:0>4x}\n", magic_enum::enum_name (pr16), magic_enum::enum_name (r8), addr );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_p8imm_r8(GbCpu::R8 r8)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = 0xFF00 + imm;

    memory[addr] = reg(r8);

    fmt::print ( "LD (0xFF00+{:0>2x}), {}\n", imm, magic_enum::enum_name(r8) );
    return InstructionTrait::no_traits();
}

InstructionTrait GbCpu::LD_r8_p8imm(GbCpu::R8 r8)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = 0xFF00 + imm;

    reg(r8, memory[addr]);

    fmt::print ( "LD {}, (0xFF00+{:0>2x})\n", magic_enum::enum_name(r8), imm );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::XOR_8imm()
{
    uint8_t imm = next_pc_byte();

    reg(R8::A, reg(R8::A) ^ imm);

    fmt::print( "XOR {:0>2x}\n", imm);
    return InstructionTrait{ R8::A };
}

InstructionTrait GbCpu::XOR_pr16(GbCpu::R16 r16)
{
    reg(R8::A, reg(R8::A) ^ memory[reg(r16)]);

    fmt::print( "XOR ({}) = {:0>4x}\n", magic_enum::enum_name(r16), reg(r16));
    return InstructionTrait{ R8::A };
}

InstructionTrait GbCpu::XOR_r8(GbCpu::R8 r8)
{
    reg(R8::A, reg(R8::A) ^ reg(r8));

    fmt::print( "XOR {}\n", magic_enum::enum_name(r8));
    return InstructionTrait{ R8::A };
}

InstructionTrait GbCpu::CP_8imm()
{
    uint8_t imm = next_pc_byte();
    int8_t r = reg(R8::A) - imm;

    registerSet.Z = r == 0;
    registerSet.N = true;
    registerSet.H = (reg(R8::A) & 0x0F) - (imm & 0x0F) < 0;
    registerSet.C = r < 0;

    fmt::print( "CP 0x{:0>2x}\n", imm);
    return InstructionTrait::affects_flags();
}

InstructionTrait GbCpu::CP_pr16(GbCpu::R16 r16)
{
    int8_t r = reg(R8::A) - memory[reg(r16)];

    registerSet.Z = r == 0;
    registerSet.N = true;
    registerSet.H = (reg(R8::A) & 0x0F) - (memory[reg(r16)] & 0x0F) < 0;
    registerSet.C = r < 0;


    fmt::print( "CP ({}) = 0x{:0>4x}\n", magic_enum::enum_name(r16), reg(r16));
    return InstructionTrait::affects_flags();
}

InstructionTrait GbCpu::CP_r8(GbCpu::R8 r8)
{
    int8_t r = reg(R8::A) - reg(r8);

    registerSet.Z = r == 0;
    registerSet.N = true;
    registerSet.H = (reg(R8::A) & 0x0F) - (reg(r8) & 0x0F) < 0;
    registerSet.C = r < 0;

    fmt::print( "CP {}\n", magic_enum::enum_name(r8));
    return InstructionTrait::affects_flags();
}

InstructionTrait GbCpu::ADD_r8_r8(GbCpu::R8 r8out, GbCpu::R8 r8in)
{
    uint8_t v0 = reg(r8out);
    uint8_t v1 = reg(r8in);

    uint16_t v2 = v0 + v1;
    reg(r8out, static_cast<uint8_t>(v2));

    registerSet.Z = v2 == 0;
    registerSet.N = 0;
    registerSet.H = (v0 & 0x0F) + (v1 & 0x0F) > 0x0F;
    registerSet.C = v2 > 0xFF;

    fmt::print( "ADD {}, {}\n", magic_enum::enum_name(r8out), magic_enum::enum_name(r8in));
    return InstructionTrait{ r8out };
}

InstructionTrait GbCpu::ADD_r8_8imm(GbCpu::R8 r8)
{
    uint8_t v0 = reg(r8);
    uint8_t v1 = next_pc_byte();

    uint16_t v2 = v0 + v1;
    reg(r8, static_cast<uint8_t>(v2));

    registerSet.Z = v2 == 0;
    registerSet.N = 0;
    registerSet.H = (v0 & 0x0F) + (v1 & 0x0F) > 0x0F;
    registerSet.C = v2 > 0xFF;

    fmt::print( "ADD {}, 0x{:0>2x}\n", magic_enum::enum_name(r8), v1);
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::ADD_r8_pr16(GbCpu::R8 r8, GbCpu::R16 r16)
{
    uint8_t v0 = reg(r8);
    uint16_t addr = reg(r16);
    uint8_t v1 = memory[addr];

    uint16_t v2 = v0 + v1;
    reg(r8, static_cast<uint8_t>(v2));

    registerSet.Z = v2 == 0;
    registerSet.N = 0;
    registerSet.H = (v0 & 0x0F) + (v1 & 0x0F) > 0x0F;
    registerSet.C = v2 > 0xFF;

    fmt::print( "ADD {}, ({}) = 0x{:>4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(r16), addr);
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::ADD_HL_r16(GbCpu::R16 r16)
{
    uint16_t v0 = reg(R16::HL);
    uint16_t v1 = reg(r16);

    uint32_t v2 = v0 + v1;
    reg(R16::HL, static_cast<uint16_t>(v2));

    registerSet.N = 0;
    registerSet.H = (v0 & 0x0FFF) + (v1 & 0x0FFF) > 0x0FFF;
    registerSet.C = v2 > 0xFFFF;

    fmt::print( "ADD HL, {}", magic_enum::enum_name(r16) );
    return InstructionTrait{ r16 };
}

InstructionTrait GbCpu::INC_r16(GbCpu::R16 r16)
{
    uint16_t v0 = reg(r16);
    v0++;
    reg(r16, v0);

    fmt::print( "INC {}\n", magic_enum::enum_name(r16) );
    return InstructionTrait{ r16 };
}

InstructionTrait GbCpu::JP_16imm()
{
    uint16_t imm = next_pc_word();
    reg ( R16::PC, imm );

    fmt::print ( "JP 0x{:0>4x}\n", imm );

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JP_C_16imm()
{
    uint16_t imm = next_pc_word();
    if (registerSet.C)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JP C 0x{:0>4x} (jumped)\n", imm );
    }
    else
    {
        fmt::print ( "JP C 0x{:0>4x} (didn't jumped)\n", imm );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JP_NC_16imm()
{
    uint16_t imm = next_pc_word();
    if (!registerSet.C)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JP NC 0x{:0>4x} (jumped)\n", imm );
    }
    else
    {
        fmt::print ( "JP NC 0x{:0>4x} (didn't jumped)\n", imm );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JP_NZ_16imm()
{
    uint16_t imm = next_pc_word();
    if (!registerSet.Z)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JP NZ 0x{:0>4x} (jumped)\n", imm );
    }
    else
    {
        fmt::print ( "JP NZ 0x{:0>4x} (didn't jumped)\n", imm );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JP_Z_16imm()
{
    uint16_t imm = next_pc_word();
    if (registerSet.Z)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JP Z 0x{:0>4x} (jumped)\n", imm );
    }
    else
    {
        fmt::print ( "JP Z 0x{:0>4x} (didn't jumped)\n", imm );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JR_8imm()
{
    uint8_t imm = next_pc_byte();
    auto current_pc = reg(R16::PC);
    reg ( R16::PC, current_pc + imm );

    fmt::print ( "JR 0x{:0>2x} = ({:0>4x})\n", imm, reg (R16::PC));

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JR_C_8imm()
{
    uint8_t imm = next_pc_byte();
    if (registerSet.C)
    {
        auto current_pc = reg(R16::PC);
        reg ( R16::PC, current_pc + imm );
        fmt::print ( "JR C 0x{:0>2x} = ({:0>4x}) (jumped)\n", imm, reg (R16::PC));
    }
    else
    {
        fmt::print ( "JR C 0x{:0>2x} = ({:0>4x}) (didn't jumped)\n", imm, reg (R16::PC));
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JR_NC_8imm()
{
    uint8_t imm = next_pc_byte();
    if (!registerSet.C)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JR NC 0x{:0>2x} = ({:0>4x}) (jumped)\n", imm, reg (R16::PC));
    }
    else
    {
        fmt::print ( "JR NC 0x{:0>2x} = ({:0>4x}) (didn't jumped)\n", imm, reg (R16::PC) );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JR_NZ_8imm()
{
    uint8_t imm = next_pc_byte();
    if (!registerSet.Z)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JR NZ 0x{:0>2x} = ({:0>4x}) (jumped)\n", imm, reg (R16::PC));
    }
    else
    {
        fmt::print ( "JR NZ 0x{:0>2x} = ({:0>4x}) (didn't jumped)\n", imm, reg (R16::PC) );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::JR_Z_8imm()
{
    uint8_t imm = next_pc_byte();
    if (registerSet.Z)
    {
        reg ( R16::PC, imm );
        fmt::print ( "JR Z 0x{:0>2x} = ({:0>4x}) (jumped)\n", imm, reg (R16::PC));
    }
    else
    {
        fmt::print ( "JR Z 0x{:0>2x} = ({:0>4x}) (didn't jumped)\n", imm, reg (R16::PC) );
    }

    return InstructionTrait::jump();
}

InstructionTrait GbCpu::DEI(bool enable)
{
    if (enable)
    {
        enable_interrupts_at = reg(R16::PC) + 1;
        fmt::print ( "EI\n" );
    }
    else
    {
        disable_interrupts_at = reg(R16::PC) + 1;
        fmt::print ( "DI\n" );
    }

    return InstructionTrait::no_traits();
}

InstructionTrait GbCpu::POP_r16(GbCpu::R16 r16)
{
    uint16_t word = pop_word();
    reg(r16, word);

    fmt::print ( "POP {}\n", magic_enum::enum_name(r16) );
    return InstructionTrait{ r16 };
}

InstructionTrait GbCpu::PUSH_r16(GbCpu::R16 r16)
{
    uint16_t word = reg(r16);
    push_word(word);

    fmt::print ( "PUSH {}\n", magic_enum::enum_name(r16) );
    return InstructionTrait::no_traits();
}


uint8_t GbCpu::next_pc_byte()
{
    uint16_t PC = registerSet.r ( R16::PC );
    uint8_t byte = memory.read_byte ( PC++ );
    registerSet.r ( R16::PC, PC );
    fetched_bytes.push_back ( byte );
    return byte;
}

uint16_t GbCpu::next_pc_word()
{
    uint16_t PC = registerSet.r ( R16::PC );

    fetched_bytes.push_back ( memory.read_byte ( PC++ ) );
    fetched_bytes.push_back ( memory.read_byte ( PC++ ) );

    registerSet.r ( R16::PC, PC );

    return order_bytes ( fetched_bytes.data() + fetched_bytes.size() - 2 );
}
