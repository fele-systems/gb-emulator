#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <pugixml.hpp>
#include <unordered_map>
#include <functional>

enum class ArgumentKind
{
    Immediate,
    Immediate_pointer,
    Register,
    Register_pointer,
    Jump_condition
};

class Argument
{
public:
    static void from_xml_node(const pugi::xml_node node, Argument& argument);
public:
    Argument() = default;
    Argument(std::string name, ArgumentKind kind, int size);
public:
    std::string name;
    ArgumentKind kind;
    int size;
};


class Opcode
{
public:
    static void from_xml_node(const pugi::xml_node node, Opcode& opcode);
public:
    Opcode() = default;
    Opcode(std::string instruction, uint8_t value, bool extended, int cycles, std::string format, int size, std::vector<Argument> arguments);
    
public:
    std::string instruction;
    uint8_t value;
    bool extended;
    int cycles;
    std::string format;
    int size;
    std::vector<Argument> arguments;
};

class OpcodeCategory
{
public:
    static void from_xml_node(const pugi::xml_node node, OpcodeCategory& category);
public:
    std::string function_name;
    std::string brief;
    std::vector<Opcode> opcodes;
};

class OpcodeManager
{
public:
    static OpcodeManager load_from_file(const char* file_name);
private:
    OpcodeManager() = default;
public:
    const std::vector<OpcodeCategory>& get_categories() const;
    void traverse_all(std::function<void(const OpcodeCategory&, const Opcode&)> consumer) const;
private:
    std::vector<OpcodeCategory> categories;
};
