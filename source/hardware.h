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
    static const std::size_t ADDRESSABLE_MEMORY_SIZE = MEM_SIZE + 8 * sizeof(std::int16_t);
    // O ponto na memória total (que inclui os registradores) em que
    // a memória do programa do Cesar começa.
    static const std::size_t MEMORY_OFFSET;
    static const std::size_t BEGIN_DISPLAY_ADDRESS;
    static const std::size_t END_DISPLAY_ADDRESS;

    std::unique_ptr<Alu> alu;

    bool halted = false;

    // Memória incluindo registradores
    union {
        std::int8_t addressable_memory[ADDRESSABLE_MEMORY_SIZE];
        struct {
            std::int16_t registers[8];
            // NOTE: Little Endian, mas deve ser tratado
            // como big endian.
            std::int8_t memory[MEM_SIZE];
        };
    };

    // Registrador de condições
    union {
        unsigned all : 4;
        struct {
            // A ordem deve ser esta, pois está por ordem
            // do bit menos significativo de "all", até o
            // mais significativo.
            //
            // all == NZVC
            //
            unsigned carry : 1;
            unsigned overflow : 1;
            unsigned zero : 1;
            unsigned negative : 1;
        };
    } condition;

    // ==============================================================
    // Métodos
    // ==============================================================

    Cpu();


    [[nodiscard]] inline std::int16_t bytes_to_word(const std::int8_t msb, const std::int8_t lsb) const { return (msb << 8) | lsb; }


    std::int16_t pop();

    void push(std::int16_t value);

    [[nodiscard]] std::int8_t get_byte(std::int16_t address) const;

    [[nodiscard]] std::int16_t get_word(std::int16_t address) const;

    void set_memory(const char *bytes, std::size_t size);

    std::int8_t *get_memory();

    bool read_memory_from_binary_file(const std::string &filename);

    void execute_next_instruction();

    std::uint16_t get_absolute_address(AddressMode mode, int register_number);

    std::int16_t get_value_from_absolute_address(std::size_t address);

    void set_value_to_absolute_address(std::int16_t value, std::size_t address);
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


    [[nodiscard]] inline bool is_negative(std::int16_t value) const { return value < 0; };


    [[nodiscard]] inline bool is_zero(std::int16_t value) const { return value == 0; }


    [[nodiscard]] inline bool is_overflow(std::int16_t a, std::int16_t b, std::int16_t c) const {
        return ((a > 0) && (b > 0) && (c < 0)) || ((a < 0) && (b < 0) && (c > 0));
    }


    [[nodiscard]] inline bool is_carry(std::int16_t a, std::int16_t b, CarryOperation operation) const {
        const auto ua = static_cast<std::uint16_t>(a);
        const auto ub = static_cast<std::uint16_t>(b);
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


    void conditional_branch(Instruction instruction, std::int8_t offset);

    std::int16_t one_operand_instruction(Instruction instruction, std::int16_t value);

    std::int16_t two_operand_instruction(Instruction instruction, std::int16_t src, std::int16_t dst);

    void ccc(std::int8_t byte);

    void scc(std::int8_t byte);

    void jmp(AddressMode mode, std::size_t absolute_address);

    void sob(std::size_t register_number, std::int8_t offset);

    void jsr(AddressMode mode, std::size_t sub_address, int register_number);

    void rts(std::int8_t word);

    std::int16_t mov(std::int16_t src);

    std::int16_t add(std::int16_t src, std::int16_t dst);

    std::int16_t sub(std::int16_t src, std::int16_t dst);

    std::int16_t cmp(std::int16_t src, std::int16_t dst);

    std::int16_t bitwise_and(std::int16_t src, std::int16_t dst);

    std::int16_t bitwise_or(std::int16_t src, std::int16_t dst);
};

#endif
