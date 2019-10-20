#ifndef HARDWARE_H
#define HARDWARE_H

#include "constants.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <memory>

struct Cpu;
struct Alu;

struct Cpu {
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

    std::unique_ptr<Alu> alu;

    // O ponto na memória total (que inclui os registradores) em que
    // a memória do programa do Cesar começa.
    static const std::size_t MEMORY_OFFSET;

    static const std::size_t BEGIN_DISPLAY_ADDRESS;
    static const std::size_t END_DISPLAY_ADDRESS;

    bool halted = false;

    Cpu();
    Byte get_byte(Word address) const;
    Word get_word(Word address) const;
    void set_memory(const char *bytes, const std::size_t size);
    Byte *get_memory();
    void read_memory_from_binary_file(const std::string &filename);
    void execute_next_instruction();

    std::size_t get_absolute_address(const AddressMode mode, const int register_number);
    Word get_absolute_value(const std::size_t address);
    void set_absolute_value(const std::size_t address, const Word value);
};



struct Alu {
    Cpu *cpu;
    Alu(Cpu *cpu);

    bool is_overflow(Word a, Word b, Word result);

    void conditional_branch(Instruction instruction, Byte offset);
    Word one_operand_instruction(Instruction instruction, Word value);
    Word two_operand_instruction(Instruction instruction, Word op1, Word op2);
    void ccc(Byte byte);
    void scc(Byte byte);
    void jmp();
    void sob();
    void jsr();
    void rts();
};

#endif
