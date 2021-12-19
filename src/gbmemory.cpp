#include "gbmemory.h"

#include <cassert>
#include <sstream>
#include <fmt/core.h>

GbMemory::GbMemory()
    : memory_buffer(0x0FFFF, 0u)
{
}

const uint8_t& GbMemory::operator[](uint16_t address) const
{
    return memory_buffer[size_t(address)];
}

uint8_t& GbMemory::operator[](uint16_t address)
{
    return memory_buffer[size_t(address)];
}

void GbMemory::load_rom(std::ifstream& gba_file)
{
    gba_file.seekg(0);    
    std::copy(std::istreambuf_iterator<char>(gba_file),
              std::istreambuf_iterator<char>(),
              memory_buffer.begin());
}

uint8_t GbMemory::read_byte(uint16_t address) const
{
    return memory_buffer[size_t(address)];
}

void GbMemory::write_word(uint16_t address, uint16_t word)
{
    memory_buffer[size_t(address)] = word & 0xFF;
    memory_buffer[size_t(address) + 1] = (word >> 8) & 0xFF;
}
uint16_t GbMemory::read_word(uint16_t address) const
{
    return memory_buffer[size_t(address)] |
        (memory_buffer[size_t(address) + 1] << 8);
}
std::string GbMemory::dump(uint16_t align, uint16_t begin, uint16_t end)
{
    auto line_start = begin - (begin % align);

    std::stringstream ss;

    while (line_start < end)
    {
        ss << fmt::format("[0x{:0>8x}] ", line_start);

        for (auto i = 0u; i < align; i++)
        {
            auto c = line_start + i;
            if (begin <= c && c < end)
            {
                ss << fmt::format("{:0<2x} ", read_byte(c));
            }
            else
            {
                ss << "?? ";
            }
        }

        line_start += align;
        ss << std::endl;
    }

    return ss.str();
}

uint16_t GbMemory::find_word(uint16_t value, uint16_t begin, uint16_t end) const
{
    if (begin >= end)
    {
        return end;
    }

    for (; begin != end; begin++)
    {
        if (read_word(begin) == value) break;
    }

    return begin;
}

std::vector<uint8_t> GbMemory::get_bytes(uint16_t start, uint16_t end) const
{
    if (start >= end)
    {
        return {};
    }
    std::vector<uint8_t> bytes;
    bytes.reserve(end - start);
    for (; start != end; start++)
    {
        bytes.push_back(read_byte(start));
    }
    return bytes;
}
