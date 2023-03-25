#include <fmt/core.h>
#include <opcodes.h>
#include <iostream>
#include <string>
#include <iomanip>

int main()
{
    OpcodeManager manager = OpcodeManager::load_from_file("opcode-template/xml-pretty-print.xml");

    std::unordered_map<std::string, Opcode> opcodes;

    manager.traverse_all([&opcodes](const auto& cat, const auto& opcode)
    {
        auto enum_str = opcode.instruction;
        if (opcode.arguments.size() > 1)
            enum_str += "_" + opcode.arguments[0].name;
        for (int i = 1; i < opcode.arguments.size(); i++)
            enum_str += "_" + opcode.arguments[1].name;
        
        std::cout << enum_str << " = ";
        if (opcode.extended)
            std::cout << fmt::format("0xCB {:#04x} ", opcode.value);
        else
            std::cout << fmt::format("{:#04x}", opcode.value);
        std::cout << std::endl;    
        opcodes[enum_str] = opcode;
    });

    
    return 0;
}
