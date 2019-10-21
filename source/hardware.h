#ifndef HARDWARE_H
#define HARDWARE_H

#include "constants.h"

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>

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
    inline Word bytes_to_word(const Byte msb, const Byte lsb) {
        return (msb << 8) | lsb;
    }

    Byte get_byte(Word address) const;

    Word get_word(Word address) const;

    void set_memory(const char *bytes, const std::size_t size);

    Byte *get_memory();

    void read_memory_from_binary_file(const std::string &filename);

    void execute_next_instruction();

    std::size_t get_absolute_address(
        const AddressMode mode, const int register_number);

    Word get_value_from_absolute_address(const std::size_t address);

    void set_value_to_absolute_address(
        const Word value, const std::size_t address);
};

struct Alu {
    Cpu *cpu;
    Alu(Cpu *cpu);

    enum CarryOperation { Plus, Minus };

    inline bool is_negative(Word value) const { return value < 0; };

    inline bool is_zero(Word value) const { return value == 0; }

    inline bool is_overflow(Word a, Word b, Word c) const {
        return ((a > 0) && (b > 0) && (c < 0)) ||
               ((a < 0) && (b < 0) && (c > 0));
    }

    inline bool is_carry(Word a, Word b, CarryOperation operation) const {
        const uint16_t ua = static_cast<uint16_t>(a);
        const uint16_t ub = static_cast<uint16_t>(b);
        uint32_t result;
        switch (operation) {
        case Plus:
            result = ua + ub;
            break;
        case Minus:
            result = ua - ub;
            break;
        }
        return (result & 0x10000) > 0;
    }

    void conditional_branch(Instruction instruction, Byte offset);
    Word one_operand_instruction(Instruction instruction, Word value);
    Word two_operand_instruction(Instruction instruction, Word op1, Word op2);
    void ccc(Byte byte);
    void scc(Byte byte);
    void jmp(Byte byte);
    void sob(Word word);
    void jsr(Word word);
    void rts(Byte word);
};

#endif
