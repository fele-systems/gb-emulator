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
    JP_N_16imm = 0xCA,
    JP_NC_16imm = 0xD2,
    JP_C_16imm = 0xDA,

    ADD_A_A = 0x87,
    ADD_A_B = 0x80,
    ADD_A_C = 0x81,
    ADD_A_D = 0x82,
    ADD_A_E = 0x83,
    ADD_A_H = 0x84,
    ADD_A_L = 0x85,
    ADD_A_pHL = 0x86,
    ADD_A_8imm = 0xC6,

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
    CP_8imm = 0xFE
};
