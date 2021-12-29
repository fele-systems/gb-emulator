#include <catch2/catch_test_macros.hpp>
#include <gbcpu.h>


TEST_CASE( "First test", "[testrun]" )
{
    std::string base_path = "../";
    std::string roms_path = base_path + "tests/";

    std::ifstream romstream(roms_path + "first-test.gb", std::ifstream::binary);
    if (!romstream) {
        throw std::runtime_error{ "Could not load rom file" };
    }

    GbCpu cpu;
    cpu.get_memory().load_rom(romstream);

    cpu.get_memory()[0xC000] = 10;
    cpu.get_memory()[0xC001] = 59;

    while (cpu.reg(Register16::PC) != 0x0000) cpu.cycle();

    REQUIRE( cpu.reg(Register8::A) == 69 );
}
