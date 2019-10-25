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
    static const std::size_t ADDRESSABLE_MEMORY_SIZE = MEM_SIZE + 8 * sizeof(Word);
    // O ponto na memória total (que inclui os registradores) em que
    // a memória do programa do Cesar começa.
    static const std::size_t MEMORY_OFFSET;
    static const std::size_t BEGIN_DISPLAY_ADDRESS;
    static const std::size_t END_DISPLAY_ADDRESS;

    std::unique_ptr<Alu> alu;

    bool halted = false;

    // Memória incluindo registradores
    union {
        Byte addressable_memory[ADDRESSABLE_MEMORY_SIZE];
        struct {
            Word registers[8];
            // NOTE: Little Endian, mas deve ser tratado
            // como big endian.
            Byte memory[MEM_SIZE];
        };
    };

    // Registrador de condições
    union {
        unsigned all : 4;
        struct {
            unsigned negative : 1;
            unsigned zero : 1;
            unsigned carry : 1;
            unsigned overflow : 1;
        };
    } condition;

    // ==============================================================
    // Métodos
    // ==============================================================

    Cpu();


    [[nodiscard]] inline Word bytes_to_word(const Byte msb, const Byte lsb) const { return (msb << 8) | lsb; }


    Word pop();

    void push(Word value);

    [[nodiscard]] Byte get_byte(Word address) const;

    [[nodiscard]] Word get_word(Word address) const;

    void set_memory(const char *bytes, std::size_t size);

    Byte *get_memory();

    bool read_memory_from_binary_file(const std::string &filename);

    void execute_next_instruction();

    std::size_t get_absolute_address(AddressMode mode, int register_number);

    Word get_value_from_absolute_address(std::size_t address);

    void set_value_to_absolute_address(Word value, std::size_t address);
};

struct Alu {
    Cpu *cpu;

    // ==============================================================
    // Métodos
    // ==============================================================

    explicit Alu(Cpu *cpu);

    enum CarryOperation {
        Plus, Minus
    };


    [[nodiscard]] inline bool is_negative(Word value) const { return value < 0; };


    [[nodiscard]] inline bool is_zero(Word value) const { return value == 0; }


    [[nodiscard]] inline bool is_overflow(Word a, Word b, Word c) const {
        return ((a > 0) && (b > 0) && (c < 0)) || ((a < 0) && (b < 0) && (c > 0));
    }


    [[nodiscard]] inline bool is_carry(Word a, Word b, CarryOperation operation) const {
        const auto ua = static_cast<UWord>(a);
        const auto ub = static_cast<UWord>(b);
        switch (operation) {
        case Plus: {
            uint32_t result = ua + ub;
            return (result & 0x10000u) > 0;
        }
        case Minus: {
            uint32_t result = ua - ub;
            return (result & 0x10000u) > 0;
        }
        }
    }


    void conditional_branch(Instruction instruction, Byte offset);

    Word one_operand_instruction(Instruction instruction, Word value);

    Word two_operand_instruction(Instruction instruction, Word src, Word dst);

    void ccc(Byte byte);

    void scc(Byte byte);

    void jmp(AddressMode mode, std::size_t absolute_address);

    void sob(std::size_t register_number, Byte offset);

    void jsr(AddressMode mode, std::size_t sub_address, int register_number);

    void rts(Byte word);

    Word mov(Word src);

    Word add(Word src, Word dst);

    Word sub(Word src, Word dst);

    Word cmp(Word src, Word dst);

    Word bitwise_and(Word src, Word dst);

    Word bitwise_or(Word src, Word dst);
};

#endif
