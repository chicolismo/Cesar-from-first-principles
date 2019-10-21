#include "hardware.h"
#include "util.h"

#include <cstring>
#include <fstream>

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

namespace hardware {

const std::size_t Cpu::MEMORY_OFFSET = offsetof(Cpu, memory);
const std::size_t Cpu::BEGIN_DISPLAY_ADDRESS = 65500;
const std::size_t Cpu::END_DISPLAY_ADDRESS = 65535;

Cpu::Cpu() {
    // Limpa os registradores
    std::memset(registers_bytes, 0, 16);
    condition.all = 0b0000;

    // Inicializa a Alu
    alu = std::make_unique<Alu>(this);
}

void Cpu::set_memory(const char *bytes, const std::size_t size) {
    const std::size_t offset{size - MEM_SIZE};
    const std::size_t max_size{MEM_SIZE < size ? MEM_SIZE : size};
    std::memcpy(memory, bytes + offset, max_size);
}

Byte *Cpu::get_memory() { return memory; }

Byte Cpu::get_byte(Word address) const {
    return memory[static_cast<uint16_t>(address)];
}

// TODO: Testar
Word Cpu::get_word(Word address) const {
    // A arquitetura do Cesar é Big Endian, portanto se espera que o byte
    // mais significativo esteja no endereço menor.
    const auto unsigned_address = static_cast<uint16_t>(address);

    if (IS_DISPLAY_ADDRESS(unsigned_address)) {
        // Se o endereço for um byte do display, retornar esse byte como
        // os bits menos significativos da palavra
        const uint8_t lsb = static_cast<uint8_t>(memory[unsigned_address + 1]);
        return static_cast<Word>(0x00FF & lsb);
    }
    else {
        const uint8_t msb = static_cast<uint8_t>(memory[unsigned_address]);
        const uint8_t lsb = static_cast<uint8_t>(memory[unsigned_address + 1]);
        return static_cast<Word>((msb << 8) | lsb);
    }
}

void Cpu::read_memory_from_binary_file(const std::string &filename) {
    std::fstream input_file(
        filename, std::ios::binary | std::ios::in | std::ios::ate);
    const std::fstream::pos_type size = input_file.tellg();

    char *buffer = new char[size];

    input_file.seekg(0);
    input_file.read(buffer, size);

    set_memory((const char *)buffer, size);

    delete[] buffer;
}

void Cpu::execute_next_instruction() {
    const uint8_t byte = static_cast<uint8_t>(get_byte(PC++));

    Instruction instruction =
        OPCODE_TO_INSTRUCTION.at(0x000F & ((0xF0 & byte) >> 4));

    if (instruction == NOP) {
        return;
    }

    if (instruction == CONDITIONAL_BRANCH) {
        Byte offset = get_byte(PC++);
        Instruction branch_instruction = OPCODE_TO_INSTRUCTION.at(byte);
        alu->conditional_branch(branch_instruction, offset);
    }
    else if (instruction == ONE_OPERAND_INSTRUCTION) {
        Instruction one_op_instruction = OPCODE_TO_INSTRUCTION.at(byte);

        const Byte next_byte = get_byte(PC++);
        const unsigned int mmm = (next_byte & 0b00111000) >> 3;
        const unsigned int rrr = (next_byte & 0b00000111);
        const AddressMode address_mode = INT_TO_ADDRESSMODE[mmm];
        const std::size_t address = get_absolute_address(address_mode, rrr);
        Word value = get_value_from_absolute_address(address);

        value = alu->one_operand_instruction(one_op_instruction, value);
        set_value_to_absolute_address(value, address);
    }
    else {
        switch (instruction) {

        case CCC:
            alu->ccc(byte);
            break;

        case SCC:
            alu->scc(byte);
            break;

        case JMP:
            alu->jmp(get_byte(PC++));
            break;

        case SOB: {
            Byte next_byte = get_byte(PC++);
            Word word = bytes_to_word(byte, next_byte);
            alu->sob(word);
        } break;

        case JSR: {
            Byte next_byte = get_byte(PC++);
            Word word = ((byte << 8) | next_byte);
            alu->jsr(word);
        } break;

        case RTS:
            alu->rts(byte);
            break;

        default:
            break;
        }

        if (IS_TWO_OPERAND_INSTRUCTION(instruction)) {
            const Byte next_byte = get_byte(PC++);
            const uint16_t word =
                static_cast<uint16_t>((byte << 8) | next_byte);
            const uint16_t mmm1 = word & (0b0000111000000000) >> 9;
            const uint16_t rrr1 = word & (0b0000000111000000) >> 6;
            const uint16_t mmm2 = word & (0b0000000000111000) >> 3;
            const uint16_t rrr2 = word & (0b0000000000000111);
            AddressMode mode1 = INT_TO_ADDRESSMODE[mmm1];
            AddressMode mode2 = INT_TO_ADDRESSMODE[mmm2];
            const uint16_t src_address = get_absolute_address(mode1, rrr1);
            const uint16_t dst_address = get_absolute_address(mode2, rrr2);

            Word src = get_value_from_absolute_address(src_address);
            Word dst = get_value_from_absolute_address(dst_address);
            Word value = alu->two_operand_instruction(instruction, src, dst);
            set_value_to_absolute_address(value, dst_address);
        }
    }
}

Word Cpu::get_value_from_absolute_address(const std::size_t address) {
    if (IS_REGISTER_ADDRESS(address)) {
        const Byte lsb = addressable_memory[address];
        const Byte msb = addressable_memory[address + 1];
        return static_cast<Word>((msb << 8) | lsb);
    }
    else if (IS_DISPLAY_ADDRESS(address)) {
        const Byte msb = addressable_memory[address];
        return static_cast<Word>(0x00FF & msb);
    }
    else {
        const Byte msb = addressable_memory[address];
        const Byte lsb = addressable_memory[address + 1];
        return static_cast<Word>((msb << 8) | lsb);
    }
}

void Cpu::set_value_to_absolute_address(
    const Word value, const std::size_t address) {
    const Byte msb = static_cast<Byte>((value & 0xFF00) >> 8);
    const Byte lsb = static_cast<Byte>(value & 0x00FF);
    if (IS_REGISTER_ADDRESS(address - MEMORY_OFFSET)) {
        addressable_memory[address] = lsb;
        addressable_memory[address + 1] = msb;
    }
    else {
        addressable_memory[address] = msb;
        addressable_memory[address + 1] = lsb;
    }
}

std::size_t Cpu::get_absolute_address(
    const AddressMode mode, const int register_number) {
    std::size_t address;

    switch (mode) {
    case AddressMode::REGISTER:
        address = register_number * 2;
        break;

    case AddressMode::REGISTER_POST_INCREMENTED:
        address = registers[register_number];
        registers[register_number] += 2;
        break;

    case AddressMode::REGISTER_PRE_DECREMENTED:
        registers[register_number] -= 2;
        address = registers[register_number];
        break;

    case AddressMode::INDEXED: {
        const Word next_word = get_word(PC);
        PC += 2;
        address = registers[register_number] + next_word;
    } break;

    case AddressMode::REGISTER_INDIRECT: {
        const uint16_t operand_address =
            static_cast<uint16_t>(registers[register_number]);
        address = get_word(operand_address);
    } break;

    case AddressMode::POST_INCREMENTED_INDIRECT: {
        const uint16_t operand_address =
            static_cast<uint16_t>(registers[register_number]);
        registers[register_number] += 2;
        address = get_word(operand_address);
    } break;

    case AddressMode::PRE_DECREMENTED_INDIRECT: {
        registers[register_number] -= 2;
        const uint16_t operand_address =
            static_cast<uint16_t>(registers[register_number]);
        address = get_word(operand_address);
    } break;

    case AddressMode::INDEXED_INDIRECT: {
        const Word next_word = get_word(PC);
        PC += 2;
        const uint16_t operand_address =
            static_cast<uint16_t>(next_word + registers[register_number]);
        address = get_word(operand_address);
    } break;
    }
    return static_cast<std::size_t>(address) + MEMORY_OFFSET;
}

} // namespace hardware

#undef PC
#undef N
#undef Z
#undef V
#undef C
#undef NZVC
#undef IS_DISPLAY_ADDRESS
#undef IS_REGISTER_ADDRESS
