#pragma once

#include "gbcpu.h"
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

enum class Opcode : uint8_t
{
    NOP = 0x00,

    LD_B_8imm = 0x06,
    LD_C_8imm = 0x0E,
    LD_D_8imm = 0x16,
    LD_E_8imm = 0x1E,
    LD_H_8imm = 0x26,
    LD_L_8imm = 0x2E,

    LD_A_A = 0x7F,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7A,
    LD_A_E = 0x7B,
    LD_A_H = 0x7C,
    LD_A_L = 0x7D,

    LD_p8imm_A = 0xE0,
    LD_A_p8imm = 0xF0,

    LD_A_pBC = 0x0A,
    LD_A_pDE = 0x1A,
    LD_A_p16imm = 0xFA,
    LD_A_8imm = 0x3E,

    LDI_A_pHL = 0x2A,

    LD_pBC_A = 0x02,
    LD_pDE_A = 0x12,
    LD_pHL_A = 0x77,
    LD_p16imm_A = 0xEA,

    LD_B_A = 0x47,
    LD_B_B = 0x40,
    LD_B_C = 0x41,
    LD_B_D = 0x42,
    LD_B_E = 0x43,
    LD_B_H = 0x44,
    LD_B_L = 0x45,

    LD_C_A = 0x4F,
    LD_C_B = 0x48,
    LD_C_C = 0x49,
    LD_C_D = 0x4A,
    LD_C_E = 0x4B,
    LD_C_H = 0x4C,
    LD_C_L = 0x4D,

    LD_D_B = 0x50,
    LD_D_C = 0x51,
    LD_D_D = 0x52,
    LD_D_E = 0x53,
    LD_D_H = 0x54,
    LD_D_L = 0x55,

    LD_E_B = 0x58,
    LD_E_C = 0x59,
    LD_E_D = 0x5A,
    LD_E_E = 0x5B,
    LD_E_H = 0x5C,
    LD_E_L = 0x5D,

    LD_L_B = 0x68,
    LD_L_C = 0x69,
    LD_L_D = 0x6A,
    LD_L_E = 0x6B,
    LD_L_H = 0x6C,
    LD_L_L = 0x6D,

    LD_H_B = 0x60,
    LD_H_C = 0x61,
    LD_H_D = 0x62,
    LD_H_E = 0x63,
    LD_H_H = 0x64,
    LD_H_L = 0x65,

    LD_A_pHL = 0x7E,
    LD_B_pHL = 0x46,
    LD_C_pHL = 0x4E,
    LD_D_pHL = 0x56,
    LD_E_pHL = 0x5E,
    LD_H_pHL = 0x66,
    LD_L_pHL = 0x6E,

    LD_pHL_B = 0x70,
    LD_pHL_C = 0x71,
    LD_pHL_D = 0x72,
    LD_pHL_E = 0x73,
    LD_pHL_H = 0x74,
    LD_pHL_L = 0x75,
    LD_pHL_8imm = 0x36,

    LD_BC_16imm = 0x01,
    LD_DE_16imm = 0x11,
    LD_HL_16imm = 0x21,
    LD_SP_16imm = 0x31,

    DI = 0xF3,
    EI = 0xFB,

    JP_16imm = 0xC3,

    JP_NZ_16imm = 0xC2,
    JP_Z_16imm = 0xCA,
    JP_NC_16imm = 0xD2,
    JP_C_16imm = 0xDA,

    JR_8imm = 0x18,

    JR_NZ_8imm = 0x20,
    JR_Z_8imm = 0x28,
    JR_NC_8imm = 0x30,
    JR_C_8imm = 0x38,

    ADD_A_A = 0x87,
    ADD_A_B = 0x80,
    ADD_A_C = 0x81,
    ADD_A_D = 0x82,
    ADD_A_E = 0x83,
    ADD_A_H = 0x84,
    ADD_A_L = 0x85,
    ADD_A_pHL = 0x86,
    ADD_A_8imm = 0xC6,

    ADD_HL_BC = 0x09,
    ADD_HL_DE = 0x19,
    ADD_HL_HL = 0x29,
    ADD_HL_SP = 0x39,

    INC_BC = 0x03,
    INC_DE = 0x13,
    INC_HL = 0x23,
    INC_SP = 0x33,

    XOR_A = 0xAF,
    XOR_B = 0xA8,
    XOR_C = 0XA9,
    XOR_D = 0XAA,
    XOR_E = 0XAB,
    XOR_H = 0XAC,
    XOR_L = 0XAD,
    XOR_pHL = 0xAE,
    XOR_p8imm = 0xEE,

    CP_A = 0XBF,
    CP_B = 0XB8,
    CP_C = 0XB9,
    CP_D = 0XBA,
    CP_E = 0XBB,
    CP_H = 0XBC,
    CP_L = 0XBD,
    CP_pHL = 0xBE,
    CP_8imm = 0xFE,

    POP_AF = 0xF1,
    POP_BC = 0xC1,
    POP_DE = 0xD1,
    POP_HL = 0xE1,

    PUSH_AF = 0xF5,
    PUSH_BC = 0xC5,
    PUSH_DE = 0xD5,
    PUSH_HL = 0xE5
};

struct OpcodeData
{
    const char* format;
};

static std::vector<OpcodeData> create_opcode_data()
{
    std::vector<OpcodeData> data(255, OpcodeData{});

    data[static_cast<uint8_t>(Opcode::NOP)] = { "NOP" };

    data[static_cast<uint8_t>(Opcode::LD_B_8imm)] = { "LD B, 0x{:>02x}" };
    data[static_cast<uint8_t>(Opcode::LD_C_8imm)] = { "LD C, 0x{:>02x}" };
    data[static_cast<uint8_t>(Opcode::LD_D_8imm)] = { "LD D, 0x{:>02x}" };
    data[static_cast<uint8_t>(Opcode::LD_E_8imm)] = { "LD E, 0x{:>02x}" };
    data[static_cast<uint8_t>(Opcode::LD_H_8imm)] = { "LD H, 0x{:>02x}" };
    data[static_cast<uint8_t>(Opcode::LD_L_8imm)] = { "LD L, 0x{:>02x}" };

    data[static_cast<uint8_t>(Opcode::LD_A_A)] = { "LD A, A" };
    data[static_cast<uint8_t>(Opcode::LD_A_B)] = { "LD A, B" };
    data[static_cast<uint8_t>(Opcode::LD_A_C)] = { "LD A, C" };
    data[static_cast<uint8_t>(Opcode::LD_A_D)] = { "LD A, D" };
    data[static_cast<uint8_t>(Opcode::LD_A_E)] = { "LD A, E" };
    data[static_cast<uint8_t>(Opcode::LD_A_H)] = { "LD A, H" };
    data[static_cast<uint8_t>(Opcode::LD_A_L)] = { "LD A, L" };

    data[static_cast<uint8_t>(Opcode::LD_p8imm_A)] = { "LD (0x{:>02x}), A" };
    data[static_cast<uint8_t>(Opcode::LD_A_p8imm)] = { "LD A, (0x{:>02x})" };

    data[static_cast<uint8_t>(Opcode::LD_A_pBC)] = { "LD A, (BC)" };
    data[static_cast<uint8_t>(Opcode::LD_A_pDE)] = { "LD A, (BC)" };
    data[static_cast<uint8_t>(Opcode::LD_A_p16imm)] = { "LD A, (0x{:>04x})" };
    data[static_cast<uint8_t>(Opcode::LD_A_8imm)] = { "LD A, {:>02x}" };

    data[static_cast<uint8_t>(Opcode::LDI_A_pHL)] = { "LDI A, (PH)" };

    data[static_cast<uint8_t>(Opcode::LD_pBC_A)] = { "LD (BC), A" };
    data[static_cast<uint8_t>(Opcode::LD_pDE_A)] = { "LD (DE), A" };
    data[static_cast<uint8_t>(Opcode::LD_pHL_A)] = { "LD (HL), A" };
    data[static_cast<uint8_t>(Opcode::LD_p16imm_A)] = { "LD 0x({:>04x}), A" };

    data[static_cast<uint8_t>(Opcode::LD_B_A)] = { "LD B, A" };
    data[static_cast<uint8_t>(Opcode::LD_B_B)] = { "LD B, B" };
    data[static_cast<uint8_t>(Opcode::LD_B_C)] = { "LD B, C" };
    data[static_cast<uint8_t>(Opcode::LD_B_D)] = { "LD B, D" };
    data[static_cast<uint8_t>(Opcode::LD_B_E)] = { "LD B, E" };
    data[static_cast<uint8_t>(Opcode::LD_B_H)] = { "LD B, H" };
    data[static_cast<uint8_t>(Opcode::LD_B_L)] = { "LD B, L" };

    data[static_cast<uint8_t>(Opcode::LD_C_A)] = { "LD C, A" };
    data[static_cast<uint8_t>(Opcode::LD_C_B)] = { "LD C, B" };
    data[static_cast<uint8_t>(Opcode::LD_C_C)] = { "LD C, C" };
    data[static_cast<uint8_t>(Opcode::LD_C_D)] = { "LD C, D" };
    data[static_cast<uint8_t>(Opcode::LD_C_E)] = { "LD C, E" };
    data[static_cast<uint8_t>(Opcode::LD_C_H)] = { "LD C, H" };
    data[static_cast<uint8_t>(Opcode::LD_C_L)] = { "LD C, L" };

    data[static_cast<uint8_t>(Opcode::LD_D_B)] = { "LD D, B" };
    data[static_cast<uint8_t>(Opcode::LD_D_C)] = { "LD D, C" };
    data[static_cast<uint8_t>(Opcode::LD_D_D)] = { "LD D, D" };
    data[static_cast<uint8_t>(Opcode::LD_D_E)] = { "LD D, E" };
    data[static_cast<uint8_t>(Opcode::LD_D_H)] = { "LD D, H" };
    data[static_cast<uint8_t>(Opcode::LD_D_L)] = { "LD D, L" };

    data[static_cast<uint8_t>(Opcode::LD_E_B)] = { "LD E, B" };
    data[static_cast<uint8_t>(Opcode::LD_E_C)] = { "LD D, C" };
    data[static_cast<uint8_t>(Opcode::LD_E_D)] = { "LD D, D" };
    data[static_cast<uint8_t>(Opcode::LD_E_E)] = { "LD D, E" };
    data[static_cast<uint8_t>(Opcode::LD_E_H)] = { "LD D, H" };
    data[static_cast<uint8_t>(Opcode::LD_E_L)] = { "LD D, L" };

    data[static_cast<uint8_t>(Opcode::LD_L_B)] = { "LD L, B" };
    data[static_cast<uint8_t>(Opcode::LD_L_C)] = { "LD L, C" };
    data[static_cast<uint8_t>(Opcode::LD_L_D)] = { "LD L, D" };
    data[static_cast<uint8_t>(Opcode::LD_L_E)] = { "LD L, E" };
    data[static_cast<uint8_t>(Opcode::LD_L_H)] = { "LD L, H" };
    data[static_cast<uint8_t>(Opcode::LD_L_L)] = { "LD L, L" };

    data[static_cast<uint8_t>(Opcode::LD_H_B = 0x60,
    data[static_cast<uint8_t>(Opcode::LD_H_C = 0x61,
    data[static_cast<uint8_t>(Opcode::LD_H_D = 0x62,
    data[static_cast<uint8_t>(Opcode::LD_H_E = 0x63,
    data[static_cast<uint8_t>(Opcode::LD_H_H = 0x64,
    data[static_cast<uint8_t>(Opcode::LD_H_L = 0x65,

    data[static_cast<uint8_t>(Opcode::LD_A_pHL = 0x7E,
    data[static_cast<uint8_t>(Opcode::LD_B_pHL = 0x46,
    data[static_cast<uint8_t>(Opcode::LD_C_pHL = 0x4E,
    data[static_cast<uint8_t>(Opcode::LD_D_pHL = 0x56,
    data[static_cast<uint8_t>(Opcode::LD_E_pHL = 0x5E,
    data[static_cast<uint8_t>(Opcode::LD_H_pHL = 0x66,
    data[static_cast<uint8_t>(Opcode::LD_L_pHL = 0x6E,

    data[static_cast<uint8_t>(Opcode::LD_pHL_B = 0x70,
    data[static_cast<uint8_t>(Opcode::LD_pHL_C = 0x71,
    data[static_cast<uint8_t>(Opcode::LD_pHL_D = 0x72,
    data[static_cast<uint8_t>(Opcode::LD_pHL_E = 0x73,
    data[static_cast<uint8_t>(Opcode::LD_pHL_H = 0x74,
    data[static_cast<uint8_t>(Opcode::LD_pHL_L = 0x75,
    data[static_cast<uint8_t>(Opcode::LD_pHL_8imm = 0x36,

    data[static_cast<uint8_t>(Opcode::LD_BC_16imm = 0x01,
    data[static_cast<uint8_t>(Opcode::LD_DE_16imm = 0x11,
    data[static_cast<uint8_t>(Opcode::LD_HL_16imm = 0x21,
    data[static_cast<uint8_t>(Opcode::LD_SP_16imm = 0x31,

    data[static_cast<uint8_t>(Opcode::DI = 0xF3,
    data[static_cast<uint8_t>(Opcode::EI = 0xFB,

    data[static_cast<uint8_t>(Opcode::JP_16imm = 0xC3,

    data[static_cast<uint8_t>(Opcode::JP_NZ_16imm = 0xC2,
    data[static_cast<uint8_t>(Opcode::JP_Z_16imm = 0xCA,
    data[static_cast<uint8_t>(Opcode::JP_NC_16imm = 0xD2,
    data[static_cast<uint8_t>(Opcode::JP_C_16imm = 0xDA,

    data[static_cast<uint8_t>(Opcode::JR_8imm = 0x18,

    data[static_cast<uint8_t>(Opcode::JR_NZ_8imm = 0x20,
    data[static_cast<uint8_t>(Opcode::JR_Z_8imm = 0x28,
    data[static_cast<uint8_t>(Opcode::JR_NC_8imm = 0x30,
    data[static_cast<uint8_t>(Opcode::JR_C_8imm = 0x38,

    data[static_cast<uint8_t>(Opcode::ADD_A_A = 0x87,
    data[static_cast<uint8_t>(Opcode::ADD_A_B = 0x80,
    data[static_cast<uint8_t>(Opcode::ADD_A_C = 0x81,
    data[static_cast<uint8_t>(Opcode::ADD_A_D = 0x82,
    data[static_cast<uint8_t>(Opcode::ADD_A_E = 0x83,
    data[static_cast<uint8_t>(Opcode::ADD_A_H = 0x84,
    data[static_cast<uint8_t>(Opcode::ADD_A_L = 0x85,
    data[static_cast<uint8_t>(Opcode::ADD_A_pHL = 0x86,
    data[static_cast<uint8_t>(Opcode::ADD_A_8imm = 0xC6,

    data[static_cast<uint8_t>(Opcode::ADD_HL_BC = 0x09,
    data[static_cast<uint8_t>(Opcode::ADD_HL_DE = 0x19,
    data[static_cast<uint8_t>(Opcode::ADD_HL_HL = 0x29,
    data[static_cast<uint8_t>(Opcode::ADD_HL_SP = 0x39,

    data[static_cast<uint8_t>(Opcode::INC_BC = 0x03,
    data[static_cast<uint8_t>(Opcode::INC_DE = 0x13,
    data[static_cast<uint8_t>(Opcode::INC_HL = 0x23,
    data[static_cast<uint8_t>(Opcode::INC_SP = 0x33,

    data[static_cast<uint8_t>(Opcode::XOR_A = 0xAF,
    data[static_cast<uint8_t>(Opcode::XOR_B = 0xA8,
    data[static_cast<uint8_t>(Opcode::XOR_C = 0XA9,
    data[static_cast<uint8_t>(Opcode::XOR_D = 0XAA,
    data[static_cast<uint8_t>(Opcode::XOR_E = 0XAB,
    data[static_cast<uint8_t>(Opcode::XOR_H = 0XAC,
    data[static_cast<uint8_t>(Opcode::XOR_L = 0XAD,
    data[static_cast<uint8_t>(Opcode::XOR_pHL = 0xAE,
    data[static_cast<uint8_t>(Opcode::XOR_p8imm = 0xEE,

    data[static_cast<uint8_t>(Opcode::CP_A = 0XBF,
    data[static_cast<uint8_t>(Opcode::CP_B = 0XB8,
    data[static_cast<uint8_t>(Opcode::CP_C = 0XB9,
    data[static_cast<uint8_t>(Opcode::CP_D = 0XBA,
    data[static_cast<uint8_t>(Opcode::CP_E = 0XBB,
    data[static_cast<uint8_t>(Opcode::CP_H = 0XBC,
    data[static_cast<uint8_t>(Opcode::CP_L = 0XBD,
    data[static_cast<uint8_t>(Opcode::CP_pHL = 0xBE,
    data[static_cast<uint8_t>(Opcode::CP_8imm = 0xFE,

    data[static_cast<uint8_t>(Opcode::POP_AF = 0xF1,
    data[static_cast<uint8_t>(Opcode::POP_BC = 0xC1,
    data[static_cast<uint8_t>(Opcode::POP_DE = 0xD1,
    data[static_cast<uint8_t>(Opcode::POP_HL = 0xE1,

    data[static_cast<uint8_t>(Opcode::PUSH_AF = 0xF5,
    data[static_cast<uint8_t>(Opcode::PUSH_BC = 0xC5,
    data[static_cast<uint8_t>(Opcode::PUSH_DE = 0xD5,
    data[static_cast<uint8_t>(Opcode::PUSH_HL = 0xE5
}

OpcodeData opcode_datas[255] = {

};
