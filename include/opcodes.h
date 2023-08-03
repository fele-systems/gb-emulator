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
    /// @brief Fills data in a opcode object from a XML node
    /// @param node XML node to be read from
    /// @param opcode Object to be written to
    static void from_xml_node(const pugi::xml_node node, Opcode& opcode);
public:
    Opcode() = default;
    Opcode(std::string instruction, uint8_t value, bool extended, int cycles, std::string format, int size, std::vector<Argument> arguments);
    /// @brief Returns a unique identifier for this opcode, based on the opcode name and parameters
    /// @return A string
    std::string to_enum_str() const;
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

/// @brief Represents the entire opcode library the CPU supports.
class OpcodeManager
{
public:
    /// @brief Loads the opcode definitions from XML file
    /// @param file_name File name of the XML
    /// @return Returns a instance of OpcodeManager
    static OpcodeManager load_from_file(const char* file_name);
private:
    OpcodeManager() = default;
public:
    /// @brief Returns the opcode categories
    /// @return Vector of categories
    const std::vector<OpcodeCategory>& get_categories() const;
    /// @brief Traverse all opcodes in all categories, calling consume for each
    /// @param consumer Function to be called for each opcode
    void traverse_all(std::function<void(const OpcodeCategory&, const Opcode&)> consumer) const;
private:
    std::vector<OpcodeCategory> categories;
};
