#include <fmt/core.h>
#include <opcodes.h>
#include <iostream>
#include <string>
#include <iomanip>

int main()
{
    OpcodeManager manager = OpcodeManager::load_from_file("opcode-template/xml-pretty-print.xml");

    std::unordered_map<std::string, Opcode> opcodes;

    manager.traverse_all([&opcodes](const auto& cat, const Opcode& opcode)
    {   
        auto enum_str = opcode.to_enum_str();
        if (opcode.extended)
            fmt::print("{} = {:#04x}\n", enum_str, opcode.value);
        else
            fmt::print("{} = 0xCB {:#04x}\n", enum_str, opcode.value);
        opcodes[enum_str] = opcode;
    });

    
    return 0;
}
