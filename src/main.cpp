#include "gbcpu.h"
#include "gbmemory.h"
#ifndef _MSC_VER
#include <bits/stdint-uintn.h>
#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <magic_enum.hpp>
#include <type_traits>
#include <sstream>
#include <array>
#include <fmt/core.h>
#include <fmt/os.h>
enum class CartridgeType : uint8_t
{
    rom                     = 0x00,
    mbc1                    = 0x01,
    mbc1_ram                = 0x02,
    mbc1_ram_battery        = 0x03,
    mbc2                    = 0x05,
    mbc2_battery            = 0x06,
    rom_ram                 = 0x08,
    rom_ram_battery         = 0x09,
    mmm01                   = 0x0b,
    mmm01_ram               = 0x0c,
    mmm01_ram_battery       = 0x0d,
    mbc3_timer_battery      = 0x0f,
    mbc3_timer_ram_battery  = 0x10,
    mbc3                    = 0x11,
    mbc3_ram                = 0x12,
    mbc3_ram_battery        = 0x13,
    mbc4                    = 0x15,
    mbc4_ram                = 0x16,
    mbc4_ram_battery        = 0x17,
    mbc5                    = 0x19,
    mbc5_ram                = 0x1a,
    mbc5_ram_battery        = 0x1b,
    mbc5_rumble             = 0x1c,
    mbc5_rumble_ram         = 0x1d,
    mbc5_rumble_ram_battery = 0x1e,
    pocket_camera           = 0xfc,
    bandai_tama5            = 0xfd,
    huc3                    = 0xfe,
    huc1_ram_battery        = 0xff
};

enum class RomSize : uint8_t
{
    s_none,
    s_2kbyte,
    s_8kbyte,
    s_32kbyte,
    s_128kbyte
};

template<typename T>
T get_from_memory(const GbMemory& memory, size_t offset)
{
    static_assert(std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t> || std::is_enum_v<T>, "T must be either uint8, uint16 or uint32");
    if constexpr ( std::is_same_v<T, uint8_t> || (std::is_enum_v<T> && sizeof(T) == 1) )
    {
        return static_cast<T>(memory[offset]);
    }
    else if constexpr ( std::is_same_v<T, uint16_t> || (std::is_enum_v<T> && sizeof(T) == 2) )
    {
        return static_cast<T>(memory[offset] | (memory[offset+1] << 8));
    }
    else if constexpr ( std::is_same_v<T, uint32_t> || (std::is_enum_v<T> && sizeof(T) == 4) )
    {
        return static_cast<T>(memory[offset] | (memory[offset+1] << 8) | (memory[offset+2] << 16) | (memory[offset+3] << 24));
    }
}

template<typename T, size_t N>
std::array<T, N> bytes_from_memory(const GbMemory& memory, size_t offset)
{
    static_assert(N > 1, "N must be higher than 1");
    static_assert(sizeof(T) == 1, "T must by a 8bit integral value");
    std::array<T, N> arr;
    // std::copy_n(memory.get_bytes(offset, N), N, arr.begin());
    for (size_t i = 0; i < N; i++)
    {
        arr[i] = memory[i+offset];
    }
    return arr;
}

struct GBRomHeader
{
public:
    static constexpr size_t rom_entrypoint_offset = 0x100;
    static constexpr size_t nintendo_logo_offset = 0x104;
    static constexpr size_t title_offset = 0x134;
    static constexpr size_t manufacurer_code_offset = 0x13f;
    static constexpr size_t game_color_offset = 0x143;
    static constexpr size_t destination_code_offset = 0x14a;
    static constexpr size_t header_checksum_offset = 0x14d;
    static constexpr size_t global_checksum_offset = 0x14e;
    static constexpr size_t version_number_offset = 0x14c;
    static constexpr size_t ram_size_offset = 0x149;
    static constexpr size_t rom_size_offset = 0x148;
    static constexpr size_t cartridge_type_offset = 0x147;
    static constexpr size_t old_licensee_offset = 0x14b;
    static constexpr size_t new_licensee_offset = 0x144;

    static constexpr size_t super_gameboy_offset = 0x143;
public:
    uint32_t rom_entrypoint;
    std::array<uint8_t, 0x2f> nintendo_logo;
    std::array<char, 0x10> title;
    uint32_t manufacurer_code;
    uint8_t game_color;
    uint8_t destination_code;
    uint8_t header_checksum;
    uint8_t global_checksum;
    uint8_t version_number;
    uint8_t ram_size;
    RomSize rom_size;
    CartridgeType cartridge_type;
    uint8_t old_licensee;
    uint16_t new_licensee;
public:
    GBRomHeader(const GbMemory& memory)
        :
        rom_entrypoint( get_from_memory<uint32_t>(memory, rom_entrypoint_offset) ),
        nintendo_logo( bytes_from_memory<uint8_t, 0x2f>(memory, nintendo_logo_offset) ),
        title( bytes_from_memory<char, 0x10>(memory, title_offset) ),
        manufacurer_code( get_from_memory<uint32_t>(memory, manufacurer_code_offset) ),
        game_color( get_from_memory<uint8_t>(memory, game_color_offset) ),
        destination_code( get_from_memory<uint8_t>(memory, destination_code_offset) ),
        header_checksum( get_from_memory<uint8_t>(memory, header_checksum_offset) ),
        global_checksum( get_from_memory<uint8_t>(memory, global_checksum_offset) ),
        version_number( get_from_memory<uint8_t>(memory, version_number_offset) ),
        ram_size( get_from_memory<uint8_t>(memory, ram_size_offset) ),
        rom_size( get_from_memory<RomSize>(memory, rom_size_offset) ),
        cartridge_type( get_from_memory<CartridgeType>(memory, cartridge_type_offset) ),
        old_licensee( get_from_memory<uint8_t>(memory, old_licensee_offset) ),
        new_licensee( get_from_memory<uint16_t>(memory, new_licensee_offset) )
    {
    }
};

std::string to_string_debug(const GBRomHeader& header)
{

    std::string str = "GBRomHeader {\n";
    auto os = std::back_inserter(str);

    fmt::format_to(os, "\trom_entrypoint: 0x{:0>8x},\n", header.rom_entrypoint);
    fmt::format_to(os, "\tnintendo_logo: blob,\n");
    fmt::format_to(os, "\ttitle: '{}',\n", std::string{ header.title.data(), header.title.size() });
    fmt::format_to(os, "\tmanufacurer_code: 0x{:0>8x},\n", header.manufacurer_code);
    fmt::format_to(os, "\tgame_color: {}(0x{:0>2x})\n", header.game_color == 0x80, header.game_color);
    fmt::format_to(os, "\tdestination_code: 0x{:0>2x}\n", header.destination_code);
    fmt::format_to(os, "\theader_checksum: 0x{:0>2x}\n", header.header_checksum);
    fmt::format_to(os, "\tglobal_checksum: 0x{:0>2x}\n", header.global_checksum);
    fmt::format_to(os, "\tversion_number: 0x{:0>2x}\n", header.version_number);
    fmt::format_to(os, "\tram_size: 0x{:0>2x}\n", header.ram_size);
    fmt::format_to(os, "\trom_size: {}(0x{:0>2x})\n", magic_enum::enum_name(header.rom_size), static_cast<uint8_t>(header.rom_size));
    fmt::format_to(os, "\tcartridge_type: {}(0x{:0>2x})\n", magic_enum::enum_name(header.cartridge_type), static_cast<uint8_t>(header.cartridge_type))  ;
    fmt::format_to(os, "\told_licensee: 0x{:0>2x}\n", header.old_licensee);
    fmt::format_to(os, "\tnew_licensee: 0x{:0>2x}\n", header.new_licensee);

    return str + "}";
}

std::string dump(uint32_t begin, uint32_t end, uint8_t align, const std::vector<uint8_t>& memory)
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
                ss << fmt::format("{:0<2x} ", memory.at(c));
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

int main()
{    
    //try
    //{

        std::string base_path = "../";
        std::string roms_path = base_path + "roms/";

        std::ifstream romstream(roms_path + "Super Mario Land (World).gb", std::ifstream::binary);
        if (!romstream) {
            throw std::runtime_error{ "Could not load rom file" };
        }

        GbCpu cpu;
        cpu.get_memory().load_rom(romstream);

        GBRomHeader header(cpu.get_memory());
        std::cout << to_string_debug( header ) << std::endl;

        for(int i = 0; i < 100; i++) cpu.cycle();
        // std::cout << dump(GBRomHeader::rom_entrypoint_offset, GBRomHeader::rom_entrypoint_offset+4, 8, memory) << std::endl;
        // std::cout << dump(header.rom_entrypoint, header.rom_entrypoint + 20, 8, memory) << std::endl;

    //} catch (std::exception& e)
    //{
    //    std::cout << e.what() << std::endl;
    //}

    std::cout << std::endl;
    return 0;
}
