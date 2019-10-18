#ifndef HARDWARE_H
#define HARDWARE_H

#include "constants.h"

#include <cstdint>
#include <cstdlib>
#include <string>

#define PC registers[7]
#define N condition.negative
#define Z condition.zero
#define V condition.overflow
#define C condition.carry
#define NZVC condition.all

class Cpu {
public:
    union {
        Byte addressable_memory[16 + MEM_SIZE];
        struct {
            union {
                Word registers[8];
                Byte registers_bytes[16];
            };
            // NOTE: Little Endian, mas deve ser tratado
            // como big endian.
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

    static std::size_t memory_offset;

    bool halted = false;

public:
    Cpu();
    Byte get_byte(Word address) const;
    Word get_word(Word address) const;
    void set_memory(const char *bytes, const std::size_t size);
    Byte *get_memory();
    void read_memory_from_binary_file(const std::string &filename);
    void execute_next_instruction();
};

#endif
