#ifndef HARDWARE_H
#define HARDWARE_H

#include "constants.h"

#include <cstdint>
#include <cstdlib>
#include <string>

//struct Word {
    //int16_t bytes;

    //Word(const Byte &msb, const Byte &lsb) {
        //bytes = static_cast<int16_t>((msb << 8) | (lsb));
    //}

    //Word operator+(const Word &other) const {
    //}
//};

class Cpu {
public:
    union {
        Byte addressable_memory[16 + MEM_SIZE];
        struct {
            union {
                Word registers[8];
                Byte registers_bytes[16];
            };
            // NOTE: Little Endian.
            Byte memory[MEM_SIZE];
        };
    };

    union {
        unsigned all : 4;
        struct {
            unsigned negative : 1;
            unsigned zero : 1;
            unsigned carry : 1;
            unsigned overflow : 1;
        };
    } condition;

    int memmory_offset;

    bool halted = false;

public:
    Cpu();
    Byte read_byte(Word address) const;
    Word read_word(Word address) const;
    void set_memory(const char *bytes, const std::size_t size);
    Byte *get_memory();
    Word get_register(int register_number) const;
    void set_register(int register_number, Word value);
    void increment_register(int register_number, Word amount);
    void read_memory_from_binary_file(const std::string &filename);
    void execute_next_instruction();
};

#endif
