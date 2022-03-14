#pragma once
#include <cstdint>
enum class Opcode : uint8_t
{
    ////////////////////////////////////////////////////
    //Miscellaneous
    ////////////////////////////////////////////////////

    NOP = 0x00, // 4 cycles 
    ////////////////////////////////////////////////////
    //8-Bit Loads
    ////////////////////////////////////////////////////

    LD_A_8imm = 0x3E, // 8 cycles 
    LD_B_8imm = 0x06, // 8 cycles 
    LD_C_8imm = 0x0E, // 8 cycles 
    LD_D_8imm = 0x16, // 8 cycles 
    LD_E_8imm = 0x1E, // 8 cycles 
    LD_H_8imm = 0x26, // 8 cycles 
    LD_L_8imm = 0x2E, // 8 cycles 
    LD_A_A = 0x7F, // 4 cycles 
    LD_A_B = 0x78, // 4 cycles 
    LD_A_C = 0x79, // 4 cycles
    LD_A_D = 0x7A, // 4 cycles 
    LD_A_E = 0x7B, // 4 cycles 
    LD_A_H = 0x7C, // 4 cycles 
    LD_A_L = 0x7D, // 4 cycles 
    LD_B_A = 0x47, // 4 cycles 
    LD_B_B = 0x40, // 4 cycles 
    LD_B_C = 0x41, // 4 cycles 
    LD_B_D = 0x42, // 4 cycles 
    LD_B_E = 0x43, // 4 cycles 
    LD_B_H = 0x44, // 4 cycles 
    LD_B_L = 0x43, // 4 cycles 
    LD_C_A = 0x4F, // 4 cycles 
    LD_C_B = 0x48, // 4 cycles 
    LD_C_C = 0x49, // 4 cycles 
    LD_C_D = 0x4A, // 4 cycles 
    LD_C_E = 0x4B, // 4 cycles 
    LD_C_H = 0x4C, // 4 cycles 
    LD_C_L = 0x4D, // 4 cycles 
    LD_D_A = 0x57, // 4 cycles 
    LD_D_B = 0x50, // 4 cycles 
    LD_D_C = 0x51, // 4 cycles 
    LD_D_D = 0x52, // 4 cycles 
    LD_D_E = 0x53, // 4 cycles 
    LD_D_H = 0x54, // 4 cycles 
    LD_D_L = 0x55, // 4 cycles 
    LD_E_A = 0x5F, // 4 cycles 
    LD_E_B = 0x58, // 4 cycles 
    LD_E_C = 0x59, // 4 cycles 
    LD_E_D = 0x5A, // 4 cycles 
    LD_E_E = 0x5B, // 4 cycles 
    LD_E_H = 0x5C, // 4 cycles 
    LD_E_L = 0x5D, // 4 cycles 
    LD_H_A = 0x67, // 4 cycles 
    LD_H_B = 0x60, // 4 cycles 
    LD_H_C = 0x61, // 4 cycles 
    LD_H_D = 0x62, // 4 cycles 
    LD_H_E = 0x63, // 4 cycles 
    LD_H_H = 0x64, // 4 cycles 
    LD_H_L = 0x65, // 4 cycles 
    LD_L_A = 0x6F, // 4 cycles 
    LD_L_B = 0x68, // 4 cycles 
    LD_L_C = 0x69, // 4 cycles 
    LD_L_D = 0x6A, // 4 cycles 
    LD_L_E = 0x6B, // 4 cycles 
    LD_L_H = 0x6C, // 4 cycles 
    LD_L_L = 0x6D, // 4 cycles 
    LD_B_pHL = 0x46, // 8 cycles 
    LD_C_pHL = 0x4E, // 8 cycles 
    LD_D_pHL = 0x56, // 8 cycles 
    LD_E_pHL = 0x5E, // 8 cycles 
    LD_H_pHL = 0x66, // 8 cycles 
    LD_L_pHL = 0x6E, // 8 cycles 
    LD_pHL_B = 0x70, // 8 cycles 
    LD_pHL_C = 0x71, // 8 cycles 
    LD_pHL_D = 0x72, // 8 cycles 
    LD_pHL_E = 0x73, // 8 cycles 
    LD_pHL_H = 0x74, // 8 cycles 
    LD_pHL_L = 0x75, // 8 cycles 
    LD_pHL_8imm = 0x36, // 12 cycles 
    LD_A_pBC = 0x0A, // 8 cycles 
    LD_A_pDE = 0x1A, // 8 cycles 
    LD_A_pHL = 0x7E, // 8 cycles 
    LD_pBC_A = 0x02, // 8 cycles 
    LD_pDE_A = 0x12, // 8 cycles 
    LD_pHL_A = 0x77, // 8 cycles 
    LD_A_p16imm = 0xFA, // 16 cycles 
    LD_p16imm_A = 0xEA, // 16 cycles 
};
