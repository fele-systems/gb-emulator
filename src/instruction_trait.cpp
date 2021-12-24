#include <instruction_trait.h>
#include <cassert>

InstructionTrait::InstructionTrait(Register8 modified_r8)
    :
    InstructionTrait(modifies_8bit_register, modified_r8, Register16::AF)
{
}

InstructionTrait::InstructionTrait(Register16 modified_r16)
    :
    InstructionTrait(modifies_16bit_register, Register8::A, modified_r16)
{
}

InstructionTrait::InstructionTrait(Flags flags, Register8 modified_r8, Register16 modified_16)
    :
    flags(flags),
    modified_r8(modified_r8),
    modified_16(modified_16)
{
}

InstructionTrait InstructionTrait::no_traits()
{
    return InstructionTrait(none, Register8::A, Register16::AF);
}

InstructionTrait InstructionTrait::jump()
{
    return InstructionTrait(Register16::PC);
}

InstructionTrait InstructionTrait::affects_flags()
{
    return InstructionTrait(modifies_flags, Register8::A, Register16::AF);
}


bool InstructionTrait::has(Flags _flags) const
{
    return (flags & _flags) == _flags;
}

Register16 InstructionTrait::get_modified_r16() const
{
    assert(has(modifies_16bit_register));
    return modified_16;
}

Register8 InstructionTrait::get_modified_r8() const
{
    assert(has(modifies_8bit_register));
    return modified_r8;
}
