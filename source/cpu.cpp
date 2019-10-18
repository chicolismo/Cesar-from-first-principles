#include "hardware.h"
#include "util.h"

#include <cstring>
#include <fstream>

std::size_t Cpu::memory_offset = offsetof(Cpu, memory);

Cpu::Cpu() {
    // Limpa os registradores
    std::memset(registers_bytes, 0, 16);
    condition.all = 0b0000;
}

void Cpu::set_memory(const char *bytes, const std::size_t size) {
    const std::size_t offset{ size - MEM_SIZE };
    const std::size_t max_size{ MEM_SIZE < size ? MEM_SIZE : size };
    std::memcpy(memory, bytes + offset, max_size);
}

Byte *Cpu::get_memory() {
    return memory;
}

Byte Cpu::get_byte(Word address) const {
    return memory[static_cast<uint16_t>(address)];
}

Word Cpu::get_word(Word address) const {
    // TODO: Verificar se o endereço está dentro da área do display.
    // Nesse caso apenas o byte mais significativo será colocado nos
    // bits menos significativos da palavra retornada.

    // A arquitetura do Cesar é Big Endian, portanto se espera que o byte mais
    // significativo esteja no endereço menor.
    const uint16_t unsigned_address = static_cast<uint16_t>(address);
    const uint8_t msb = memory[unsigned_address];
    const uint8_t lsb = memory[unsigned_address + 1];
    const uint16_t unsigned_word = static_cast<uint16_t>((msb << 8) | lsb);
    return static_cast<Word>(unsigned_word);
}


void Cpu::read_memory_from_binary_file(const std::string &filename) {
    std::fstream input_file(filename, std::ios::binary | std::ios::in | std::ios::ate);
    const std::fstream::pos_type size = input_file.tellg();

    char *buffer = static_cast<char *>(malloc(size));

    input_file.seekg(0);
    input_file.read(buffer, size);

    set_memory((const char *) buffer, size);

    free(buffer);
}


void Cpu::execute_next_instruction() {
    const uint8_t byte = static_cast<uint8_t>(get_byte(PC++));

    Instruction instruction = OPCODE_TO_INSTRUCTION.at(0x000F & ((0xF0 & byte) >> 4));

    /*
    std::string_view instruction_name;
    if (instruction == CONDITIONAL_BRANCH || instruction == ONE_OPERAND_INSTRUCTION) {
        instruction_name = INSTRUCTION_NAMES[OPCODE_TO_INSTRUCTION.at(byte)];
    }
    else {
        instruction_name = INSTRUCTION_NAMES[instruction];
    }
    std::cout << instruction_name;
    return;
    */

    if (instruction == Instruction::NOP) {
        return;
    }

    if (instruction == CONDITIONAL_BRANCH) {
        Byte offset = get_byte(PC++);

        Instruction branch_instruction = OPCODE_TO_INSTRUCTION.at(byte);

        switch (branch_instruction) {

        case BR:
            PC += offset;
            break;
        case BNE:
            if (Z == 0) {
                PC += offset;
            }
            break;
        case BEQ:
            if (Z == 1) {
                PC += offset;
            }
            break;
        case BPL:
            if (N == 0) {
                PC += offset;
            }
            break;
        case BMI:
            if (N == 1) {
                PC += offset;
            }
            break;
        case BVC:
            if (V == 0) {
                PC += offset;
            }
            break;
        case BVS:
            if (V == 1) {
                PC += offset;
            }
            break;
        case BCC:
            if (C == 0) {
                PC += offset;
            }
            break;
        case BCS:
            if (C == 1) {
                PC += offset;
            }
            break;
        case BGE:
            if (N == V) {
                PC += offset;
            }
            break;
        case BLT:
            if (N != V) {
                PC += offset;
            }
            break;
        case BGT:
            if (N == V && Z == 0) {
                PC += offset;
            }
            break;
        case BLE:
            if (N != V || Z == 1) {
                PC += offset;
            }
            break;
        case BHI:
            if (C == 0 && Z == 0) {
                PC += offset;
            }
            break;
        case BLS:
            if (C == 1 || Z == 1) {
                PC += offset;
            }
            break;
        default:
            return; // NOP
        }
    }

    if (instruction == ONE_OPERAND_INSTRUCTION) {
        Instruction one_op_instruction = OPCODE_TO_INSTRUCTION.at(byte);

        switch (one_op_instruction) {
        case CLR:
            break;
        case NOT:
            break;
        case INC:
            break;
        case DEC:
            break;
        case NEG:
            break;
        case TST:
            break;
        case ROR:
            break;
        case ROL:
            break;
        case ASR:
            break;
        case ASL:
            break;
        case ADC:
            break;
        case SBC:
            break;
        default:
            return; // NOP
        }
    }

    // TODO: Usar ponteiros para endereçamento

    switch (instruction) {
    case NOP:
        break;

    case CCC:
        NZVC = ~(byte & 0x0F);
        break;
    case SCC:
        NZVC = (byte & 0x0F);
        break;

    case JMP:
        break;
    case SOB:
        break;
    case JSR:
        break;
    case RTS:
        break;

    case MOV:
        break;
    case ADD:
        break;
    case SUB:
        break;
    case CMP:
        break;
    case AND:
        break;
    case OR:
        break;

    default:
        return; // NOP
    }
}
