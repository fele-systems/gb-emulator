#include "gbcpu.h"
#include "gbmemory.h"
#include "opcodes.h"
#include "bit_utils.h"
#include <iostream>
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/color.h>
#include <errno.h>
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

    fmt::print ( fmt::fg ( fmt::color::green ), "0x{:0<4x} | ", address );
    for ( auto b : fetched_bytes ) {
        fmt::print ( "{:0<2x} ", b );
    }
    fmt::print ( "\n" );

    using namespace magic_enum::bitwise_operators;

    if ( traits.has(InstructionTrait::modifies_8bit_register) ) {
        fmt::print ( fmt::fg ( fmt::color::red ),
                     "{}: {:0<2x} -> {:0<2x}\n",
                     magic_enum::enum_name ( traits.get_modified_r8() ),
                     registerSetCopy.r( traits.get_modified_r8() ),
                     reg ( traits.get_modified_r8() ) );
    }

    if ( traits.has(InstructionTrait::modifies_16bit_register) ) {
        fmt::print ( fmt::fg ( fmt::color::red ),
                     "{}: {:0<4x} -> {:0<4x}\n",
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

InstructionTrait GbCpu::execute_next()
{
    auto instruction = static_cast<Opcode> ( next_pc_byte() );
    switch ( instruction ) {
    case Opcode::NOP:
        // cycles 4
        fmt::print("NOP\n");
        return InstructionTrait::no_traits();

    ///////////////////////////////////////////////////////////////////////////////////
    ////// 8-Bit Loads
    ///////////////////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------------------
    //---- LD r8, 8imm
    // Put 8-Bit immediate value into 8-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::LD_A_8imm:
        // 8 cycles
        return LD_r8_8imm(R8::A);

    case Opcode::LD_B_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::B );

    case Opcode::LD_C_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::C );

    case Opcode::LD_D_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::D );

    case Opcode::LD_E_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::E );

    case Opcode::LD_H_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::H );

    case Opcode::LD_L_8imm:
        // cycles 8
        return LD_r8_8imm ( R8::L );

    //---------------------------------------------------------------------------------
    //---- LD r8_out, r8_in
    // Put 8-Bit register (in) into 8-Bit register (out)
    //---------------------------------------------------------------------------------

    //*********************************************************************************
    //**** Load into A
    //*********************************************************************************

    case Opcode::LD_A_A:
        // cycles 4
        return LD_r8_r8(R8::A, R8::L);

    case Opcode::LD_A_B:
        // cycles 4
        return LD_r8_r8(R8::A, R8::B);

    case Opcode::LD_A_C:
        // cycles 4
        return LD_r8_r8(R8::A, R8::C);

    case Opcode::LD_A_D:
        // cycles 4
        return LD_r8_r8(R8::A, R8::D);

    case Opcode::LD_A_E:
        // cycles 4
        return LD_r8_r8(R8::A, R8::E);

    case Opcode::LD_A_H:
        // cycles 4
        return LD_r8_r8(R8::A, R8::H);

    case Opcode::LD_A_L:
        // cycles 4
        return LD_r8_r8(R8::A, R8::L);

    //*********************************************************************************
    //**** Load into B
    //*********************************************************************************

    case Opcode::LD_B_A:
        // 4 cycles
        return LD_r8_r8(R8::B, R8::A);

    case Opcode::LD_B_B:
        // cycles 4
        return LD_r8_r8(R8::B, R8::B);

    case Opcode::LD_B_C:
        // cycles 4
        return LD_r8_r8(R8::B, R8::C);

    case Opcode::LD_B_D:
        // cycles 4
        return LD_r8_r8(R8::B, R8::D);

    case Opcode::LD_B_E:
        // cycles 4
        return LD_r8_r8(R8::B, R8::E);

    case Opcode::LD_B_H:
        // cycles 4
        return LD_r8_r8(R8::B, R8::H);

    case Opcode::LD_B_L:
        // cycles 4
        return LD_r8_r8(R8::B, R8::L);

    //*********************************************************************************
    //**** Load into C
    //*********************************************************************************

    case Opcode::LD_C_A:
        // cycles 4
        return LD_r8_r8(R8::C, R8::A);

    case Opcode::LD_C_B:
        // cycles 4
        return LD_r8_r8(R8::C, R8::B);

    case Opcode::LD_C_C:
        // cycles 4
        return LD_r8_r8(R8::C, R8::C);

    case Opcode::LD_C_D:
        // cycles 4
        return LD_r8_r8(R8::C, R8::D);

    case Opcode::LD_C_E:
        // cycles 4
        return LD_r8_r8(R8::C, R8::E);

    case Opcode::LD_C_H:
        // cycles 4
        return LD_r8_r8(R8::C, R8::H);

    case Opcode::LD_C_L:
        // cycles 4
        return LD_r8_r8(R8::C, R8::L);

    //*********************************************************************************
    //**** Load into D
    //*********************************************************************************

    case Opcode::LD_D_B:
        // cycles 4
        return LD_r8_r8(R8::D, R8::B);

    case Opcode::LD_D_C:
        // cycles 4
        return LD_r8_r8(R8::D, R8::C);

    case Opcode::LD_D_D:
        // cycles 4
        return LD_r8_r8(R8::D, R8::D);

    case Opcode::LD_D_E:
        // cycles 4
        return LD_r8_r8(R8::D, R8::E);

    case Opcode::LD_D_H:
        // cycles 4
        return LD_r8_r8(R8::D, R8::H);

    case Opcode::LD_D_L:
        // cycles 4
        return LD_r8_r8(R8::D, R8::L);

    //*********************************************************************************
    //**** Load into E
    //*********************************************************************************

    case Opcode::LD_E_B:
        // cycles 4
        return LD_r8_r8(R8::E, R8::B);

    case Opcode::LD_E_C:
        // cycles 4
        return LD_r8_r8(R8::E, R8::C);

    case Opcode::LD_E_D:
        // cycles 4
        return LD_r8_r8(R8::E, R8::D);

    case Opcode::LD_E_E:
        // cycles 4
        return LD_r8_r8(R8::E, R8::E);

    case Opcode::LD_E_H:
        // cycles 4
        return LD_r8_r8(R8::E, R8::H);

    case Opcode::LD_E_L:
        // cycles 4
        return LD_r8_r8(R8::E, R8::L);

    //*********************************************************************************
    //**** Load into L
    //*********************************************************************************

    case Opcode::LD_L_B:
        // cycles 4
        return LD_r8_r8(R8::L, R8::B);

    case Opcode::LD_L_C:
        // cycles 4
        return LD_r8_r8(R8::L, R8::C);

    case Opcode::LD_L_D:
        // cycles 4
        return LD_r8_r8(R8::L, R8::D);

    case Opcode::LD_L_E:
        // cycles 4
        return LD_r8_r8(R8::L, R8::E);

    case Opcode::LD_L_H:
        // cycles 4
        return LD_r8_r8(R8::L, R8::H);

    case Opcode::LD_L_L:
        // cycles 4
        return LD_r8_r8(R8::L, R8::L);

    //*********************************************************************************
    //**** Load into H
    //*********************************************************************************

    case Opcode::LD_H_B:
        // cycles 4
        return LD_r8_r8(R8::H, R8::B);

    case Opcode::LD_H_C:
        // cycles 4
        return LD_r8_r8(R8::H, R8::C);

    case Opcode::LD_H_D:
        // cycles 4
        return LD_r8_r8(R8::H, R8::D);

    case Opcode::LD_H_E:
        // cycles 4
        return LD_r8_r8(R8::H, R8::E);

    case Opcode::LD_H_H:
        // cycles 4
        return LD_r8_r8(R8::H, R8::H);

    case Opcode::LD_H_L:
        // cycles 4
        return LD_r8_r8(R8::H, R8::L);

    //---------------------------------------------------------------------------------
    //---- LD r8, (HL)
    // Put value from memory at HL into 8-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::LD_A_pHL:
        // cycles 8
        return LD_r8_pr16(R8::A, R16::HL);

    case Opcode::LD_B_pHL:
        // cycles 8
        return LD_r8_pr16(R8::B, R16::HL);

    case Opcode::LD_C_pHL:
        // cycles 8
        return LD_r8_pr16(R8::C, R16::HL);

    case Opcode::LD_D_pHL:
        // cycles 8
        return LD_r8_pr16(R8::D, R16::HL);

    case Opcode::LD_E_pHL:
        // cycles 8
        return LD_r8_pr16(R8::E, R16::HL);

    case Opcode::LD_H_pHL:
        // cycles 8
        return LD_r8_pr16(R8::H, R16::HL);

    case Opcode::LD_L_pHL:
        // cycles 8
        return LD_r8_pr16(R8::L, R16::HL);

    //---------------------------------------------------------------------------------
    //---- LD A, (r16)
    // Put value from memory at r16 into 8-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::LD_A_pBC:
        // 8 cycles
        return LD_r8_pr16(R8::A, R16::BC);

    case Opcode::LD_A_pDE:
        // 8 cycles
        return LD_r8_pr16(R8::A, R16::DE);

    case Opcode::LD_A_p16imm:
        // 16 cycles
        return LD_r8_p16imm(R8::A);

    //---------------------------------------------------------------------------------
    //---- LD (r16), A
    // Put value from 8-Bit register into memory at r16
    //---------------------------------------------------------------------------------

    case Opcode::LD_pBC_A:
        // 8 cycles
        return LD_pr16_r8(R16::BC, R8::A);

    case Opcode::LD_pDE_A:
        // 8 cycles
        return LD_pr16_r8(R16::DE, R8::A);

    case Opcode::LD_pHL_A:
        // 8 cycles
        return LD_pr16_r8(R16::HL, R8::A);

    case Opcode::LD_p16imm_A:
        // 16 cycles
        return LD_p16imm_r8(R8::A);

    //---------------------------------------------------------------------------------
    //---- LD (0xFF00+8imm), A
    // Put value from 8-Bit register into memory at 0xFF00 + 8-Bit immediate
    //---------------------------------------------------------------------------------

    case Opcode::LD_p8imm_A:
        // 12 cycles
        return LD_p8imm_r8(R8::A);

    //---------------------------------------------------------------------------------
    //---- LD A, (0xFF00+8imm)
    // Put value from memory at 0xFF00 + 8-Bit immediate into 8-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::LD_A_p8imm:
        // 12 cycles
        return LD_r8_p8imm(R8::A);

    //---------------------------------------------------------------------------------
    //---- LD (HL), r8
    // Put value from 8-Bit register into memory at HL
    //---------------------------------------------------------------------------------

    case Opcode::LD_pHL_B:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::B);

    case Opcode::LD_pHL_C:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::C);

    case Opcode::LD_pHL_D:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::D);

    case Opcode::LD_pHL_E:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::E);

    case Opcode::LD_pHL_H:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::H);

    case Opcode::LD_pHL_L:
        // cycles 8
        return LD_pr16_r8(R16::HL, R8::L);

    //---------------------------------------------------------------------------------
    //---- LD (HL), 8imm
    // Put value 8-Bit immediate value into memory at HL
    //---------------------------------------------------------------------------------

    case Opcode::LD_pHL_8imm:
        // cycles 12
        return LD_pr16_8imm(R16::HL);

    //---------------------------------------------------------------------------------
    //---- LDI A, (HL)
    // Put value pointed by HL into A. Increment HL.
    //---------------------------------------------------------------------------------

    case Opcode::LDI_A_pHL:
        // 8 cycles
        return LDI_r8_p16r(R8::A, R16::HL);

    ///////////////////////////////////////////////////////////////////////////////////
    ////// 16-Bit Loads
    ///////////////////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------------------
    //---- LD r16, 16imm
    // Loads 16-Bit immediate value into 16-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::LD_BC_16imm:
        // cycles 12
        // LD BC, 16imm
        return LD_r16_16imm ( R16::BC );

    case Opcode::LD_DE_16imm:
        // cycles 12
        // LD DE, 16imm
        return LD_r16_16imm ( R16::DE );

    case Opcode::LD_HL_16imm:
        // cycles 12
        // LD HL, 16imm
        return LD_r16_16imm ( R16::HL );

    case Opcode::LD_SP_16imm:
        // cycles 12
        // LD SP, 16imm
        return LD_r16_16imm ( R16::SP );

    ///////////////////////////////////////////////////////////////////////////////////
    ////// Jumps
    ///////////////////////////////////////////////////////////////////////////////////

    case Opcode::JP_16imm:
        // cycles 12
        // JP 16imm
        return JP_16imm( );

    ///////////////////////////////////////////////////////////////////////////////////
    ////// 8-Bit ALU
    ///////////////////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------------------
    //---- ADD r8, r8
    // Add 8-Bit register into 8-Bit register
    //---------------------------------------------------------------------------------

    case Opcode::ADD_A_A:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::A);

    case Opcode::ADD_A_B:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::B);

    case Opcode::ADD_A_C:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::C);

    case Opcode::ADD_A_D:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::D);

    case Opcode::ADD_A_E:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::E);

    case Opcode::ADD_A_H:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::H);

    case Opcode::ADD_A_L:
        // 4 cycles
        return ADD_r8_r8(R8::A, R8::L);

    case Opcode::ADD_A_pHL:
        // 8 cycles
        return ADD_r8_pr16(R8::A, R16::HL);

    case Opcode::ADD_A_8imm:
        // 8 cycles
        return ADD_r8_8imm(R8::A);

    //---------------------------------------------------------------------------------
    //---- XOR r8
    // XORs A with 8-Bit register, store in A
    //---------------------------------------------------------------------------------

    case Opcode::XOR_A:
        // cycles 4
        return XOR_r8(R8::A);
        break;
    case Opcode::XOR_B:
        // cycles 4
        return XOR_r8(R8::B);
        break;
    case Opcode::XOR_C:
        // cycles 4
        return XOR_r8(R8::C);
        break;
    case Opcode::XOR_D:
        // cycles 4
        return XOR_r8(R8::D);
        break;
    case Opcode::XOR_E:
        // cycles 4
        return XOR_r8(R8::E);
        break;
    case Opcode::XOR_H:
        // cycles 4
        return XOR_r8(R8::H);
        break;
    case Opcode::XOR_L:
        // cycles 4
        return XOR_r8(R8::L);
        break;
    case Opcode::XOR_pHL:
        // cycles 4
        return XOR_pr16(R16::HL);
        break;
    case Opcode::XOR_p8imm:
        // cycles 4
        return XOR_8imm();
        break;

    //---------------------------------------------------------------------------------
    //---- CP r8
    // Compares A with 8-Bit register. Modifies flags
    // Z = A - r8 == 0
    // N = true
    // H = A4bit - r84bit < 0
    // C = A - r8 < 0
    //---------------------------------------------------------------------------------

    case Opcode::CP_A:
        // 4 cycles
        return CP_r8(R8::A);

    case Opcode::CP_B:
        // 4 cycles
        return CP_r8(R8::B);

    case Opcode::CP_C:
        // 4 cycles
        return CP_r8(R8::C);

    case Opcode::CP_D:
        // 4 cycles
        return CP_r8(R8::D);

    case Opcode::CP_E:
        // 4 cycles
        return CP_r8(R8::E);

    case Opcode::CP_H:
        // 4 cycles
        return CP_r8(R8::H);

    case Opcode::CP_L:
        // 4 cycles
        return CP_r8(R8::L);

    case Opcode::CP_pHL:
        // 8 cycles
        return CP_pr16(R16::HL);

    case Opcode::CP_8imm:
        // 8 cycles
        return CP_8imm();

    ///////////////////////////////////////////////////////////////////////////////////
    ////// Misc
    ///////////////////////////////////////////////////////////////////////////////////

    case Opcode::DI:
        // cycles 4
        return DEI(false);
    case Opcode::EI:
        // cycles 4
        return DEI(true);

    default:
        throw std::runtime_error{ fmt::format("Unkown opcode: {:0<2x}", static_cast<int>( instruction )) };
    }
}

InstructionTrait GbCpu::LD_r8_r8 (R8 r8out, R8 r8in)
{
    reg (r8out, reg(r8in));

    fmt::print ( "LD {}, {}\n", magic_enum::enum_name (r8out), magic_enum::enum_name (r8in) );

    return InstructionTrait{ r8out };
}

InstructionTrait GbCpu::LD_r8_8imm (R8 r8)
{
    uint8_t imm = next_pc_byte();
    reg ( r8, imm );

    fmt::print ( "LD {}, 0x{:0<2x}\n", magic_enum::enum_name ( r8 ), imm );

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_r16_16imm ( R16 r16 )
{
    uint16_t imm = next_pc_word();
    reg ( r16, imm );

    fmt::print ( "LD {}, 0x{:0<4x}\n", magic_enum::enum_name ( r16 ), imm );

    return InstructionTrait{ r16 };
}

InstructionTrait GbCpu::LDI_r8_p16r(GbCpu::R8 r8, GbCpu::R16 r16)
{
    uint16_t addr = reg (r16);
    reg ( r8, memory[addr] );
    reg ( r16, addr + 1 );

    fmt::print ( "LDI {}, ({}) = 0x{:0<4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(r16), addr);

    return InstructionTrait{ r8 };
}


InstructionTrait GbCpu::LD_p16imm_r8(R8 r8)
{
    uint16_t imm = next_pc_word();
    memory[imm] = reg(r8);

    fmt::print ( "LD ({:0<4x}), {}\n", imm, magic_enum::enum_name(r8) );

    return InstructionTrait::no_traits();
}


InstructionTrait GbCpu::LD_r8_p16imm(R8 r8)
{
    uint16_t imm = next_pc_word();
    reg( r8, imm );

    fmt::print ( "LD {}, ({:0<4x})\n", magic_enum::enum_name(r8), imm );

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_r8_pr16(R8 r8, R16 r16)
{
    uint16_t addr = reg( r16 );
    uint8_t byte = memory[addr];
    reg ( r8, byte );

    fmt::print ( "LD {}, ({}) = 0x{:0<4x}\n", magic_enum::enum_name (r8), magic_enum::enum_name (r16), addr );

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_pr16_r8(R16 r16, R8 r8)
{
    uint16_t addr = reg( r16 );
    memory[addr] = reg (r8);

    fmt::print ( "LD ({}), {} = 0x{:0<4x}\n", magic_enum::enum_name (r16), magic_enum::enum_name (r8), addr );

    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::LD_pr16_8imm(R16 r16)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = reg ( r16 );

    memory[addr] = imm;

    fmt::print ( "LD ({}), 0x{:0<2x} = 0x{:0<4x}\n", magic_enum::enum_name (r16), imm, addr );

    return InstructionTrait::no_traits();
}

InstructionTrait GbCpu::LD_p8imm_r8(GbCpu::R8 r8)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = 0xFF00 + imm;

    memory[addr] = reg(r8);

    fmt::print ( "LD (0xFF00+{:0<2x}), {}\n", imm, magic_enum::enum_name(r8) );
    return InstructionTrait::no_traits();
}

InstructionTrait GbCpu::LD_r8_p8imm(GbCpu::R8 r8)
{
    uint8_t imm = next_pc_byte();
    uint16_t addr = 0xFF00 + imm;

    reg(r8, memory[addr]);

    fmt::print ( "LD {}, (0xFF00+{:0<2x})\n", magic_enum::enum_name(r8), imm );
    return InstructionTrait{ r8 };
}

InstructionTrait GbCpu::XOR_8imm()
{
    uint8_t imm = next_pc_byte();

    reg(R8::A, reg(R8::A) ^ imm);

    fmt::print( "XOR {:0<2x}\n", imm);
    return InstructionTrait{ R8::A };
}

InstructionTrait GbCpu::XOR_pr16(GbCpu::R16 r16)
{
    reg(R8::A, reg(R8::A) ^ memory[reg(r16)]);

    fmt::print( "XOR ({}) = {:0<4x}\n", magic_enum::enum_name(r16), reg(r16));
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

    fmt::print( "CP 0x{:0<2x}\n", imm);
    return InstructionTrait::affects_flags();
}

InstructionTrait GbCpu::CP_pr16(GbCpu::R16 r16)
{
    int8_t r = reg(R8::A) - memory[reg(r16)];

    registerSet.Z = r == 0;
    registerSet.N = true;
    registerSet.H = (reg(R8::A) & 0x0F) - (memory[reg(r16)] & 0x0F) < 0;
    registerSet.C = r < 0;


    fmt::print( "CP ({}) = 0x{:0<4x}\n", magic_enum::enum_name(r16), reg(r16));
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

    fmt::print( "ADD {}, 0x{:0<2x}\n", magic_enum::enum_name(r8), v1);
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

    fmt::print( "ADD {}, ({}) = 0x{:<4x}\n", magic_enum::enum_name(r8), magic_enum::enum_name(r16), addr);
    return InstructionTrait{ r8 };
}


InstructionTrait GbCpu::JP_16imm()
{
    uint16_t imm = next_pc_word();
    reg ( R16::PC, imm );

    fmt::print ( "JP 0x{:0<4x}\n", imm );

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
