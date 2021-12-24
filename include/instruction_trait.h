#pragma once

#include <registers.h>

class InstructionTrait
{
public:
    enum Flags
    {
        none                        = 0,
        modifies_8bit_register      = 1 << 0,
        modifies_16bit_register     = 1 << 1,
        modifies_flags              = 1 << 2,
        writes_to_memory            = 1 << 3
    };
public:
    bool has(Flags _flags) const;
    Register16 get_modified_r16() const;
    Register8 get_modified_r8() const;
public:
    InstructionTrait(Register8 modified_r8);
    InstructionTrait(Register16 modified_r16);
    InstructionTrait(Flags flags, Register8 modified_r8, Register16 modified_16);
    static InstructionTrait no_traits();
    static InstructionTrait jump();
    static InstructionTrait affects_flags();
private:
    const Flags flags;
    const Register8 modified_r8;
    const Register16 modified_16;
};
