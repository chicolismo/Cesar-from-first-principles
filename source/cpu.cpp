#include "hardware.h"
#include "util.h"

#include <cstring>
#include <fstream>

#define PC get_register(7)
#define INCREMENT_PC increment_register(7, 1)
#define ADD_TO_PC(value) increment_register(7, (value))

Cpu::Cpu() {
    memmory_offset = offsetof(Cpu, memory);
    condition.all = 0b0000;
}

Word Cpu::get_register(int register_number) const {
    return registers[register_number];

    // register_number *= 2;
    // const Byte lsb = registers_bytes[register_number];
    // const Byte msb = registers_bytes[register_number + 1];
    // return static_cast<Word>((msb << 8) | lsb);
}

void Cpu::set_register(int register_number, Word value) {
    registers[register_number] = value;

    // const Byte lsb = static_cast<Byte>(value & 0x00FF);
    // const Byte msb = static_cast<Byte>((value & 0xFF) >> 8);
    // register_number *= 2;
    // registers_bytes[register_number] = lsb;
    // registers_bytes[register_number + 1] = msb;
}

void Cpu::increment_register(int register_number, Word amount) {
    registers[register_number] += amount;

    // Word register_value = get_register(register_number);
    // register_value += amount;
    // set_register(register_number, register_value);
}

void Cpu::set_memory(const char *bytes, const std::size_t size) {
    const std::size_t offset{ size - MEM_SIZE };
    const std::size_t max_size{ MEM_SIZE < size ? MEM_SIZE : size };
    std::memcpy(memory, bytes + offset, max_size);
}

Byte *Cpu::get_memory() {
    return memory;
}

Byte Cpu::read_byte(Word address) const {
    return memory[static_cast<uint16_t>(address)];
}

Word Cpu::read_word(Word address) const {
    // TODO: Verificar se o endereço está dentro da área do display.
    // Nesse caso apenas o byte mais significativo será colocado nos
    // bits menos significativos da palavra retornada.

    // A arquitetura do Cesar é Big Endian, portanto se espera que o byte mais
    // significativo esteja no endereço menor.
    uint16_t unsigned_address = static_cast<uint16_t>(address);
    uint8_t msb = memory[unsigned_address];
    uint8_t lsb = memory[unsigned_address + 1];
    uint16_t unsigned_word = static_cast<uint16_t>((msb << 8) | lsb);
    return static_cast<Word>(unsigned_word);
}


void Cpu::read_memory_from_binary_file(const std::string &filename) {
    std::fstream input_file(filename, std::ios::binary | std::ios::in | std::ios::ate);
    std::fstream::pos_type size = input_file.tellg();

    char *buffer = static_cast<char *>(malloc(size));

    input_file.seekg(0);
    input_file.read(buffer, size);

    set_memory((const char *) buffer, size);

    free(buffer);
}


void Cpu::execute_next_instruction() {
    uint8_t byte = (uint8_t) read_byte(PC);
    INCREMENT_PC;

    Instruction instruction = OPCODE_TO_INSTRUCTION.at(0x000F & ((0xF0 & byte) >> 4));

    if (instruction == Instruction::NOP) {
        return;
    }

    if (instruction == CONDITIONAL_BRANCH || instruction == ONE_OPERAND_INSTRUCTION) {
        instruction = OPCODE_TO_INSTRUCTION.at(byte);
    }

    std::cout << INSTRUCTION_NAMES[instruction] << '\n';
    return;

    switch (instruction) {
    case NOP:
        break;
    case CCC: {
        unsigned nzvc = ~(byte & 0x0F);
        condition.all = nzvc;
    } break;
    case SCC: {
        unsigned nzvc = (byte & 0x0F);
        condition.all = nzvc;
    } break;
    default:
        return; // NOP
    }

    if (instruction == CONDITIONAL_BRANCH) {
        switch (instruction) {

        case BR: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            ADD_TO_PC(offset);
        } break;

        case BNE: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.zero == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BEQ: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.zero == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        case BPL: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BMI: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        case BVC: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.overflow == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BVS: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.overflow == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        case BCC: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.carry == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BCS: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.carry == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        case BGE: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative == condition.overflow) {
                ADD_TO_PC(offset);
            }
        } break;

        case BLT: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative != condition.overflow) {
                ADD_TO_PC(offset);
            }
        } break;

        case BGT: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative == condition.overflow && condition.zero == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BLE: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.negative != condition.overflow || condition.zero == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        case BHI: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.carry == 0 && condition.zero == 0) {
                ADD_TO_PC(offset);
            }
        } break;

        case BLS: {
            Byte offset = read_byte(PC);
            INCREMENT_PC;
            if (condition.carry == 1 || condition.zero == 1) {
                ADD_TO_PC(offset);
            }
        } break;

        default:
            return; // NOP
        }
    }

    if (instruction == JMP) {
        // return;
    }

    if (instruction == ONE_OPERAND_INSTRUCTION) {
        instruction = OPCODE_TO_INSTRUCTION.at(byte);
    }


    // TODO: Operações de 2 operandos...

    // TODO: Usar ponteiros para endereçamento
}
