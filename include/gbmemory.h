#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

class GbMemory
{
public:
    GbMemory();

    /**
     * @brief Loads the rom @0x0000
     *
     * @param gb_file File stream to read bytes from
     */
    void load_rom(std::ifstream& gb_file);
    
    /**
     * @brief Reads a word (16 bit value) from memory.
     * 
     * This includes the bytes at address to address + 1, so the next word would be at address + 2.
     * 
     * @param address Address to read.
     * @return uint16_t Word at address.
     */
    uint16_t read_word(uint16_t address) const;

    uint8_t read_byte(uint16_t address) const;

    void write_word(uint16_t address, uint16_t word);

    std::string dump(uint16_t align, uint16_t begin, uint16_t end);

    uint16_t find_word(uint16_t value, uint16_t begin, uint16_t end) const;

    std::vector<uint8_t> get_bytes(uint16_t start, uint16_t end) const;

    const uint8_t& operator[](uint16_t address) const;
    uint8_t& operator[](uint16_t address);
public:
    static constexpr uint32_t rom_base = 0x08000000;
    static constexpr uint32_t word_size = 4;
private:
    std::vector<uint8_t> memory_buffer;
};
