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

    void cycle();
    GbMemory& get_memory();

private:
    InstructionTrait execute_next();

    /**
     * Loads the 16-Bit immediate value into the specified 16-Bit register.
     * i.e. r16 = 16imm
     * @param r16 The target register
     */
    InstructionTrait LD_r16_16imm (R16 r16);
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

    InstructionTrait JP_16imm();

    InstructionTrait XOR_r8(R8 r8);

    InstructionTrait XOR_pr16(R16 r16);

    InstructionTrait XOR_8imm();

    InstructionTrait ADD_r8_r8(R8 r8out, R8 r8in);
    InstructionTrait ADD_r8_pr16(R8 r8, R16 r16);
    InstructionTrait ADD_r8_8imm(R8 r8);

    InstructionTrait CP_r8(R8 r8);

    InstructionTrait CP_pr16(R16 r16);

    InstructionTrait CP_8imm();

    InstructionTrait DEI(bool enable);

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

