#include <opcode.h>    
#include <magic_enum.hpp>
#include <cstdlib>
#include <cassert>

Argument::Argument(std::string name, ArgumentKind kind, int size)
    :
    name(std::move(name)),
    kind(kind),
    size(size)
{}

void Argument::from_xml_node(const pugi::xml_node node, Argument& argument)
{
    argument.name = node.attribute("name").value();
    const std::string kind_str = node.attribute("kind").value();
    argument.kind = magic_enum::enum_cast<ArgumentKind>(kind_str).value_or(ArgumentKind::Jump_condition);
    argument.size = node.attribute("size").as_int();
}

Opcode::Opcode(std::string instruction, uint8_t value, bool extended, int cycles, std::string format, int size, std::vector<Argument> arguments)
    :
    instruction(std::move(instruction)),
    value(value),
    extended(extended),
    cycles(cycles),
    format(std::move(format)),
    size(size),
    arguments(std::move(arguments))
{}


void Opcode::from_xml_node(const pugi::xml_node node, Opcode& opcode)
{
    opcode.instruction = node.attribute("instruction").value();
    auto value_str = std::string{ node.attribute("value").as_string() };
    char* value_str_end = nullptr;
    opcode.value = std::strtol(value_str.c_str(), &value_str_end, 16);
    // opcode.extended = node.attribute("extended").as_bool();
    opcode.extended = value_str_end == value_str.c_str()+value_str.size();
    opcode.cycles = node.attribute("cycles").as_int();
    opcode.format = node.attribute("format").value();
    opcode.size = node.attribute("size").as_int();
    for (auto argument_node : node.children("argument"))
    {
        opcode.arguments.emplace_back();
        Argument::from_xml_node(argument_node, opcode.arguments.back());
    }
}

std::string Opcode::to_enum_str() const
{
    auto enum_str = instruction;
    if (arguments.size() >= 1)
        enum_str += "_" + arguments[0].name;
    for (std::size_t i = 1; i < arguments.size(); i++)
        enum_str += "_" + arguments[1].name;
    return enum_str;
}

void OpcodeCategory::from_xml_node(const pugi::xml_node node, OpcodeCategory& category)
{
    category.function_name = node.attribute("category").value();
    category.brief = node.attribute("brief").value();
    for (auto node : node.children("opcode"))
    {
        category.opcodes.emplace_back();
        Opcode::from_xml_node(node, category.opcodes.back());
    }
    assert(!category.opcodes.empty());
}
OpcodeManager OpcodeManager::load_from_file(const char* file_name)
{
    OpcodeManager manager;
    pugi::xml_document doc;
    auto result = doc.load_file(file_name);
    assert(result.status == pugi::status_ok);
    auto opcodes = doc.child("opcodes");
    assert(opcodes);
    for (auto node : opcodes.children("function-category"))
    {
        manager.categories.emplace_back();
        OpcodeCategory::from_xml_node(node, manager.categories.back());
    }
    return manager;
}


const std::vector<OpcodeCategory>& OpcodeManager::get_categories() const
{
    return categories;
}

void OpcodeManager::traverse_all(std::function<void(const OpcodeCategory&, const Opcode&)> consumer) const
{
    for (const auto& category : categories)
    {
        for (const auto& opcode : category.opcodes)
        {
            consumer(category, opcode);
        }
    }
}
