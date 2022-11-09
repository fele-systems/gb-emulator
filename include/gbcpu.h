#pragma once

#include <gbmemory.h>
#include <magic_enum.hpp>
#include <array>
#include <stdexcept>
#include <registers.h>
#include <instruction_trait.h>


class GbCpu
{
public:
    using R8 = Register8;
    using R16 = Register16;
    GbCpu();

    uint8_t reg(Register8 num) const { return registerSet.r(num); }
    void reg(Register8 num, uint8_t value) { return registerSet.r(num, value); }
    void reg(Register16 num, uint16_t value) { registerSet.r(num, value); }
    uint16_t reg(Register16 num) const { return registerSet.r(num); }


    void push_word(uint16_t word);
    uint16_t pop_word();

    void cycle();
    GbMemory& get_memory();

private:
    InstructionTrait execute_next();

    InstructionTrait NOP ()
    {
        return InstructionTrait::no_traits();
    }
    /**
     * Loads the 16-Bit immediate value into the specified 16-Bit register.
     * i.e. r16 = 16imm
     * @param r16 The target register
     */
    InstructionTrait LD_r16_16imm (R16 r16);
    /**
     * Loads the 16-Bit register value into the specified 16-Bit register.
     * i.e. r16 = 16imm
     * @param r16out The target register
     * @param r16in The source register
     */
    InstructionTrait LD_r16_r16 (R16 r16out, R16 r16in);
    /**
     * Loads the value at 16-Bit register into the specified 8-Bit register. Increments 16-Bit register afterwards.
     * i.e. r8 = (r16++)
     * @param r8 The target register
     * @param r16 The source pointer register
     */
    InstructionTrait LDI_r8_p16r (R8 r8, R16 r16);
    /**
     * Loads the 8-Bit immediate value into the specified 8-Bit register.
     * i.e. r8 = LD_pr16_8imm
     * @param r8 The target register
     *
     */
    InstructionTrait LD_r8_8imm (R8 r8);
    /**
     * Loads the value from a 8-Bit register into another.
     * i.e. r8out = r8in
     * @param r8out The target register
     * @param r8in The source register
     */
    InstructionTrait LD_r8_r8 (R8 r8out, R8 r8in);
    /**
     * Loads the value at 8-Bit register pointer into another 8-Bit register
     * i.e. r8out = (r8in)
     * @param r8 The target register
     * @param pr8 The source register pointer
     */
    InstructionTrait LD_r8_pr8 (R8 r8, R8 pr8);
    /**
     * Loads the value from 8-Bit register into memory at 8-Bit register pointer
     * i.e. r8out = (r8in)
     * @param pr8 The target register pointer
     * @param r8 The source register
     */
    InstructionTrait LD_pr8_r8 (R8 pr8, R8 r8);

    /**
     * Loads the 8-Bit value from the memory pointed by 16-Bit register into 8-Bit register.
     * i.e. r8 = (r16)
     * @param r8 The target register
     * @param r16 The source pointer register
     */
    InstructionTrait LD_r8_pr16(R8 r8, R16 r16);
    /**
     * Loads the 8-Bit register into the memory pointed by 16-Bit register.
     * i.e. (r16) = r8
     * @param r16 The target pointer register
     * @param r8 The source register
     */
    InstructionTrait LD_pr16_r8(R16 r16, R8 r8);
    /**
     * Loads the 8-Bit immediate value into memory pointed by 16-bit register.
     * i.e. (r16) = 8imm
     * @param r16 The target pointer register
     *
     */
    InstructionTrait LD_pr16_8imm(R16 r16);
    /**
     * Loads the 8-Bit value pointed by 16-Bit register into 8-bit register. Decrements 16-Bit register
     * i.e. (r16) = 8imm
     * @param r8 The target register
     * @param r16 The source pointer register
     */
    InstructionTrait LDD_r8_pr16(R8 r8, R16 r16);
    /**
     * Loads the 8-Bit register into value pointed by 16-Bit register. Decrements 16-Bit register
     * i.e. (r16) = 8imm
     * @param r16 The source pointer register
     * @param r8 The target register
     */
    InstructionTrait LDD_pr16_r8(R16 r16, R8 r8);
        /**
     * Loads the 8-Bit value pointed by 16-Bit register into 8-bit register. Increments 16-Bit register
     * i.e. (r16) = 8imm
     * @param r8 The target register
     * @param r16 The source pointer register
     */
    InstructionTrait LDI_r8_pr16(R8 r8, R16 r16);
    /**
     * Loads the 8-Bit register into value pointed by 16-Bit register. Increments 16-Bit register
     * i.e. (r16) = 8imm
     * @param r16 The source pointer register
     * @param r8 The target register
     */
    InstructionTrait LDI_pr16_r8(R16 r16, R8 r8);

    /**
     * Loads the memory pointed by 16-bit immediate value into 8-Bit register.
     * i.e. (16imm) = r8
     * @param r8 The source register
     *
     */
    InstructionTrait LD_p16imm_r8(R8 r8);
    /**
     * Loads the value from memory at 16-Bit immediate into 8-Bit register
     *
     * i.e. r8 = (16imm)
     * @param r8 The target register
     */
    InstructionTrait LD_r8_p16imm(R8 r8);
    /**
     * Loads the value at 0xFF00 + 8-Bit immediate into 8-Bit register
     * @param r8 The target register
     */
    InstructionTrait LD_r8_p8imm(R8 r8);
    /**
     * Loads the 8-Bit register into memory at 0xFF00 + 8-Bit immediate
     * @param r8 The source register
     */
    InstructionTrait LD_p8imm_r8(R8 r8);

    /**
     * Adds 8-Bit signed immediate to 16-Bit register and save to another 16-Bit register
     * @param r16out Target register
     * @param r16in Source register
     */
    InstructionTrait LD_r16_r16_8imm(R16 r16out, R16 r16in);

    /**
     * Saves SP to address at 16-Bit immediate
     * @param r16 Must be SP
     */ 
    InstructionTrait LD_p16imm_r16(R16 r16);

    InstructionTrait JP_r16(R16 r16);
    InstructionTrait JP_16imm();

    InstructionTrait JP_NZ_16imm();
    InstructionTrait JP_Z_16imm();
    InstructionTrait JP_NC_16imm();
    InstructionTrait JP_C_16imm();

    InstructionTrait JP_8imm();

    InstructionTrait JP_NZ_8imm();
    InstructionTrait JP_Z_8imm();
    InstructionTrait JP_NC_8imm();
    InstructionTrait JP_C_8imm();

    InstructionTrait JR_8imm();

    InstructionTrait JR_NZ_8imm();
    InstructionTrait JR_Z_8imm();
    InstructionTrait JR_NC_8imm();
    InstructionTrait JR_C_8imm();

    InstructionTrait CALL_16imm();
    InstructionTrait CALL_NZ_16imm();
    InstructionTrait CALL_Z_16imm();
    InstructionTrait CALL_NC_16imm();
    InstructionTrait CALL_C_16imm();

    InstructionTrait ADD_r8_r8(R8 r8out, R8 r8in);
    InstructionTrait ADD_r8_pr16(R8 r8, R16 r16);
    InstructionTrait ADD_r8_8imm(R8 r8);

    InstructionTrait ADD_r16_r16(R16 r16out, R16 r16in);
    InstructionTrait ADD_HL_r16(R16 r16);
    InstructionTrait ADD_r16_8imm(R16 r16);


    /**
     * Adds 8-Bit register into another 8-Bit register plus carry.
     */
    InstructionTrait ADC_r8_r8(R8 r8out, R8 r8in);
    InstructionTrait ADC_r8_pr16(R8 r8, R16 pr16);
    InstructionTrait ADC_r8_8imm(R8 r8);
    

    InstructionTrait SUB_r8_r8(R8 r8out, R8 r8in);
    InstructionTrait SUB_r8_pr16(R8 r8, R16 pr16);
    InstructionTrait SUB_r8_8imm(R8 r8);

    InstructionTrait SBC_r8_r8(R8 r8out, R8 r8in);
    InstructionTrait SBC_r8_pr16(R8 r8, R16 pr16);
    InstructionTrait SBC_r8_8imm(R8 r8);

    InstructionTrait INC_r16(R16 r16);
    InstructionTrait INC_r8(R8 r8);
    InstructionTrait INC_pr16(R16 pr16);

    InstructionTrait DEC_r16(R16 r16);
    InstructionTrait DEC_r8(R8 r8);
    InstructionTrait DEC_pr16(R16 pr16);

    InstructionTrait AND_r8(R8 r8);
    InstructionTrait AND_pr16(R16 r16);
    InstructionTrait AND_8imm();

    InstructionTrait OR_r8(R8 r8);
    InstructionTrait OR_pr16(R16 r16);
    InstructionTrait OR_8imm();

    InstructionTrait XOR_r8(R8 r8);
    InstructionTrait XOR_pr16(R16 r16);
    InstructionTrait XOR_8imm();

    InstructionTrait CP_r8(R8 r8);
    InstructionTrait CP_pr16(R16 r16);
    InstructionTrait CP_8imm();

    InstructionTrait DEI(bool enable);

    InstructionTrait DAA();
    InstructionTrait CPL();
    InstructionTrait CCF();
    InstructionTrait SCF();
    InstructionTrait HALT();
    InstructionTrait DI();
    InstructionTrait EI();
    InstructionTrait RLCA();
    InstructionTrait RLA();
    InstructionTrait RRCA();
    InstructionTrait RRA();
    InstructionTrait RET();
    InstructionTrait RETI();
    InstructionTrait RET_NZ();
    InstructionTrait RET_Z();
    InstructionTrait RET_NC();
    InstructionTrait RET_C();

    InstructionTrait POP_r16(R16 r16);
    InstructionTrait PUSH_r16(R16 r16);

    InstructionTrait SWAP_r8(R8 r8);

    /**
     * Fetches the next byte from PC. Increment PC by 1.
     */
    uint8_t next_pc_byte();
    /**
     * Fetches the next word from PC. Increment PC by 2.
     */
    uint16_t next_pc_word();
private:
    RegisterSet registerSet { 0x0001, 0x0013, 0x00D8, 0x014D, 0xFFFE, 0x0100 };
    GbMemory memory;
    std::vector<uint8_t> fetched_bytes;
    int disable_interrupts_at = 0;
    int enable_interrupts_at = 0;
};

