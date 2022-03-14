#pragma once

#include <string>
#include "opcodes.h"


class GbLogger
{
    virtual void log_instruction(Opcode opcode) = 0;
};
