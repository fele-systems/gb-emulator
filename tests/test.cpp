#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <gbcpu.h>
#include <filesystem>

const auto base_path = std::filesystem::path(ROOT_DIR);
const auto roms_path = base_path / "tests";

TEST_CASE( "First test", "[testrun]" )
{
    std::ifstream romstream(roms_path / "first-test.gb", std::ifstream::binary);
    if (!romstream) {
        throw std::runtime_error{ "Could not load rom file" };
    }

    GbCpu cpu;
    cpu.get_memory().load_rom(romstream);
    romstream.close();

    cpu.get_memory()[0xC000] = 10;
    cpu.get_memory()[0xC001] = 59;

    while (cpu.reg(Register16::PC) != 0x0000) cpu.cycle();

    REQUIRE( cpu.reg(Register8::A) == 69 );
}

TEST_CASE( "Find String Size", "[testrun]" )
{
    std::ifstream romstream(roms_path / "string-len.gb", std::ifstream::binary);
    if (!romstream) {
        throw std::runtime_error{ "Could not load rom file" };
    }

    GbCpu cpu;
    cpu.get_memory().load_rom(romstream);
    romstream.close();

    cpu.reg(Register16::SP, 0xCFF0);

    std::string str = "Hello, world!";
    cpu.get_memory().write_string(0xC0A0, str);
    cpu.push_word(0xC0A0);

    while (cpu.reg(Register16::PC) != 0x0000) cpu.cycle();

    auto size = cpu.pop_word();

    REQUIRE( size == str.size() );
}

TEST_CASE( "Copy String", "[testrun]" )
{
    std::ifstream romstream(roms_path / "string-copy.gb", std::ifstream::binary);
    if (!romstream) {
        throw std::runtime_error{ "Could not load rom file" };
    }

    GbCpu cpu;
    cpu.get_memory().load_rom(romstream);
    romstream.close();

    cpu.reg(Register16::SP, 0xCFF0);

    std::string str = "Hello, world!";
    cpu.get_memory().write_string(0xC0A0, str);
    cpu.push_word(0xC0A0);
    cpu.push_word(str.size());
    cpu.push_word(0xC0F0);

    while (cpu.reg(Register16::PC) != 0x0000) cpu.cycle();



    REQUIRE( str == cpu.get_memory().read_string(0xC0F0, str.size()) );
}
