#include "hardware.h"

#include <cstring>
#include <fstream>

namespace cesar::hardware {

#define SP registers[6]
#define PC registers[7]
#define N condition.negative
#define Z condition.zero
#define V condition.overflow
#define C condition.carry
#define NZVC condition.all
#define IS_DISPLAY_ADDRESS(address)                                            \
    (address) >= BEGIN_DISPLAY_ADDRESS && (address) <= END_DISPLAY_ADDRESS
#define IS_REGISTER_ADDRESS(address) (address) < MEMORY_OFFSET
#define IS_TWO_OPERAND_INSTRUCTION(inst)                                       \
    (inst) == MOV || (inst) == ADD || (inst) == SUB || (inst) == CMP ||        \
            (inst) == AND || (inst) == OR

const std::size_t Cpu::MEMORY_OFFSET =
        offsetof(Cpu, memory) - offsetof(Cpu, addressable_memory);
const std::size_t Cpu::BEGIN_DISPLAY_ADDRESS = 65500;
const std::size_t Cpu::END_DISPLAY_ADDRESS = 65535;

Cpu::Cpu() {
    // Limpa a memória
    std::memset(addressable_memory, 0, ADDRESSABLE_MEMORY_SIZE);
    condition.all = 0b0000;

    // Inicializa a Alu
    // alu = std::make_unique<Alu>(this);
    alu = new Alu(this);
}

Cpu::~Cpu() { delete alu; }

void Cpu::set_memory(const char *bytes, const std::size_t size) {
    const std::size_t offset{size - MEM_SIZE};
    const std::size_t maxsize{MEM_SIZE < size ? MEM_SIZE : size};
    std::memcpy(memory, bytes + offset, maxsize);
}

std::int8_t *Cpu::get_memory() { return memory; }

std::int8_t Cpu::get_byte(std::int16_t address) const {
    return memory[static_cast<std::uint16_t>(address)];
}

// TODO: Testar
std::int16_t Cpu::get_word(std::int16_t address) const {
    // A arquitetura do Cesar é Big Endian, portanto se espera que o byte
    // mais significativo esteja no endereço menor.
    const auto unsigned_address = static_cast<std::uint16_t>(address);

    if (IS_DISPLAY_ADDRESS(unsigned_address)) {
        // Se o endereço for um byte do display, retornar esse byte como
        // os bits menos significativos da palavra
        const auto lsb =
                static_cast<std::uint8_t>(memory[unsigned_address + 1]);
        return static_cast<std::int16_t>(0x00FFu & lsb);
    }
    else {
        const auto msb = static_cast<std::uint8_t>(memory[unsigned_address]);
        const auto lsb =
                static_cast<std::uint8_t>(memory[unsigned_address + 1]);
        return static_cast<std::int16_t>(msb << 8 | lsb);
    }
}

bool Cpu::read_memory_from_binary_file(const std::string &filename) {
    std::fstream input_file(
            filename, std::ios::binary | std::ios::in | std::ios::ate);

    if (!input_file.is_open()) {
        return false;
    }

    const std::fstream::pos_type size = input_file.tellg();

    if (static_cast<unsigned>(size) < MEM_SIZE) {
        return false;
    }

    char *buffer = new char[size];

    input_file.seekg(0);
    input_file.read(buffer, size);

    set_memory((const char *) buffer, size);

    delete[] buffer;

    return true;
}

void Cpu::push(const std::int16_t word) {
    const auto unsigned_word = static_cast<std::uint16_t>(word);
    const std::int8_t a = (unsigned_word >> 8u);
    const std::int8_t b = (unsigned_word & 0x00FFu);
    SP -= 2;
    memory[SP] = a;
    memory[SP + 1] = b;
}

std::int16_t Cpu::pop() {
    const std::int8_t a = memory[SP];
    const std::int8_t b = memory[SP + 1];
    SP += 2;
    return bytes_to_word(a, b);
}

void Cpu::execute_next_instruction() {
    // TODO: Precisa do cast?
    const auto unsigned_byte = static_cast<std::uint8_t>(get_byte(PC++));

    const Instruction instruction =
            OPCODE_TO_INSTRUCTION.at(unsigned_byte >> 4u & 0x0Fu);

    if (instruction == NOP) {
        return;
    }

    if (instruction == HLT) {
        halted = true;
        return;
    }

    if (instruction == CONDITIONAL_BRANCH) {
        const Instruction branch_instruction =
                OPCODE_TO_INSTRUCTION.at(unsigned_byte);
        const std::int8_t offset = get_byte(PC++);
        alu->conditional_branch(branch_instruction, offset);
    }
    else if (instruction == ONE_OPERAND_INSTRUCTION) {
        const Instruction one_op_instruction =
                OPCODE_TO_INSTRUCTION.at(unsigned_byte);

        const auto next_unsigned_byte =
                static_cast<std::uint8_t>(get_byte(PC++));
        const int mmm = (next_unsigned_byte & 0b00111000) >> 3;
        const int rrr = (next_unsigned_byte & 0b00000111);
        const AddressMode address_mode = INT_TO_ADDRESSMODE[mmm];
        const std::size_t address = get_absolute_address(address_mode, rrr);
        std::int16_t value = get_value_from_absolute_address(address);

        value = alu->one_operand_instruction(one_op_instruction, value);
        set_value_to_absolute_address(value, address);
    }
    else {
        switch (instruction) {
            case CCC:
                alu->ccc(unsigned_byte);
                break;

            case SCC:
                alu->scc(unsigned_byte);
                break;

            case JMP: {
                const auto next_unsigned_byte =
                        static_cast<std::uint8_t>(get_byte(PC++));
                const int mmm = (next_unsigned_byte & 0b00111000) >> 3;
                const int rrr = (next_unsigned_byte & 0b00000111);
                const AddressMode mode = INT_TO_ADDRESSMODE[mmm];
                const std::size_t absolute_address =
                        get_absolute_address(mode, rrr);
                alu->jmp(mode, absolute_address);
            } break;

            case SOB: {
                const int rrr = (unsigned_byte & 0b00000111);
                const std::int8_t offset = get_byte(PC++);
                alu->sob(rrr, offset);
            } break;

            case JSR: {
                const auto next_unsigned_byte =
                        static_cast<std::uint8_t>(get_byte(PC++));
                const std::int16_t word =
                        bytes_to_word(unsigned_byte, next_unsigned_byte);
                const int reg = (word & (0b0000011100000000)) >> 8;
                const int mmm = (word & (0b0000000000111000)) >> 3;
                const int rrr = (word & (0b0000000000000111));
                const AddressMode mode = INT_TO_ADDRESSMODE[mmm];
                const std::size_t sub_address = get_absolute_address(mode, rrr);
                alu->jsr(mode, sub_address, reg);
            } break;

            case RTS:
                alu->rts(unsigned_byte);
                break;

            default: {
                if (IS_TWO_OPERAND_INSTRUCTION(instruction)) {
                    const auto next_unsigned_byte =
                            static_cast<std::uint8_t>(get_byte(PC++));
                    const auto uword = static_cast<std::uint16_t>(
                            (unsigned_byte << 8u) | next_unsigned_byte);
                    const int mmm1 = uword & (0b0000111000000000) >> 9;
                    const int rrr1 = uword & (0b0000000111000000) >> 6;
                    const int mmm2 = uword & (0b0000000000111000) >> 3;
                    const int rrr2 = uword & (0b0000000000000111);
                    const AddressMode mode1 = INT_TO_ADDRESSMODE[mmm1];
                    const AddressMode mode2 = INT_TO_ADDRESSMODE[mmm2];
                    const std::size_t src_address =
                            get_absolute_address(mode1, rrr1);
                    const std::size_t dst_address =
                            get_absolute_address(mode2, rrr2);

                    const std::int16_t src =
                            get_value_from_absolute_address(src_address);
                    const std::int16_t dst =
                            get_value_from_absolute_address(dst_address);
                    const std::int16_t value =
                            alu->two_operand_instruction(instruction, src, dst);
                    if (instruction != Instruction::CMP) {
                        // Não precisa escrever de volta o valor da comparação
                        set_value_to_absolute_address(value, dst_address);
                    }
                }
            } break;
        }
    }
}

std::int16_t Cpu::get_value_from_absolute_address(const std::size_t address) {
    if (IS_REGISTER_ADDRESS(address)) {
        const std::int8_t lsb = addressable_memory[address];
        const std::int8_t msb = addressable_memory[address + 1];
        return static_cast<std::int16_t>((msb << 8) | lsb);
    }
    else if (IS_DISPLAY_ADDRESS(address)) {
        const std::int8_t msb = addressable_memory[address];
        return static_cast<std::int16_t>(0x00FF & msb);
    }
    else {
        const std::int8_t msb = addressable_memory[address];
        const std::int8_t lsb = addressable_memory[address + 1];
        return static_cast<std::int16_t>((msb << 8) | lsb);
    }
}

void Cpu::set_value_to_absolute_address(
        const std::int16_t value, const std::size_t address) {
    auto msb = static_cast<std::int8_t>((value & 0xFF00) >> 8);
    auto lsb = static_cast<std::int8_t>(value & 0x00FF);
    if (IS_REGISTER_ADDRESS(address - MEMORY_OFFSET)) {
        addressable_memory[address] = lsb;
        addressable_memory[address + 1] = msb;
    }
    else {
        addressable_memory[address] = msb;
        addressable_memory[address + 1] = lsb;
    }
}

std::uint16_t Cpu::get_absolute_address(
        const AddressMode mode, const int register_number) {
    std::uint16_t address{0u};

    switch (mode) {
        case AddressMode::REGISTER:
            address = register_number * 2;
            break;

        case AddressMode::REGISTER_POST_INCREMENTED:
            address = static_cast<std::uint16_t>(registers[register_number]);
            registers[register_number] += 2;
            break;

        case AddressMode::REGISTER_PRE_DECREMENTED:
            registers[register_number] -= 2;
            address = static_cast<std::uint16_t>(registers[register_number]);
            break;

        case AddressMode::INDEXED: {
            const std::int16_t next_word = get_word(PC);
            PC += 2;
            address = static_cast<std::uint16_t>(
                    registers[register_number] + next_word);
        } break;

        case AddressMode::REGISTER_INDIRECT: {
            address = static_cast<std::uint16_t>(registers[register_number]);
        } break;

        case AddressMode::POST_INCREMENTED_INDIRECT: {
            const auto operand_address =
                    static_cast<std::uint16_t>(registers[register_number]);
            registers[register_number] += 2;
            address = get_word(operand_address);
        } break;

        case AddressMode::PRE_DECREMENTED_INDIRECT: {
            registers[register_number] -= 2;
            const auto operand_address =
                    static_cast<std::uint16_t>(registers[register_number]);
            address = get_word(operand_address);
        } break;

        case AddressMode::INDEXED_INDIRECT: {
            const std::int16_t next_word = get_word(PC);
            PC += 2;
            const auto operand_address = static_cast<std::uint16_t>(
                    next_word + registers[register_number]);
            address = get_word(operand_address);
        } break;
    }

    return static_cast<std::size_t>(address) + MEMORY_OFFSET;
}

#undef PC
#undef N
#undef Z
#undef V
#undef C
#undef NZVC
#undef IS_DISPLAY_ADDRESS
#undef IS_REGISTER_ADDRESS

} // namespace cesar::hardware
