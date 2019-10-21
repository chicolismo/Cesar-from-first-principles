#include "hardware.h"

#define PC cpu->registers[7]
#define NZVC cpu->condition.all
#define N cpu->condition.negative
#define Z cpu->condition.zero
#define V cpu->condition.overflow
#define C cpu->condition.carry

namespace hardware {

Alu::Alu(Cpu *cpu) : cpu(cpu) {}

void Alu::conditional_branch(Instruction instruction, Byte offset) {
    switch (instruction) {
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
        break;
    }
}

Word Alu::one_operand_instruction(Instruction instruction, Word value) {
    // TODO. Garantir que todos as condições foram testadas.

    switch (instruction) {
    case CLR:
        value = 0;
        N = 0;
        Z = 1;
        C = 0;
        V = 0;
        break;

    case NOT:
        value = ~value;
        N = value < 0;
        Z = value == 0;
        C = 1;
        V = 0;
        break;

    case INC:
        V = is_overflow(value, 1, value + 1);
        C = static_cast<uint16_t>(value) == 0xFFFF;
        value += 1;
        N = value < 0;
        Z = value == 0;
        break;

    case DEC:
        V = is_overflow(value, 1, value - 1);
        C = value == 0;
        value -= 1;
        N = value < 0;
        Z = value == 0;
        break;

    case NEG:
        V = is_overflow(value, 1, value + 1);
        C = value == 0; // TODO: Confirmar isto!
        value = -value;
        N = is_negative(value);
        Z = is_zero(value);
        break;

    case TST:
        N = is_negative(value);
        Z = is_zero(value);
        C = 0;
        V = 0;
        break;

    case ROR: {
        uint16_t temp = static_cast<uint16_t>(value);
        uint16_t lsb = (temp & 0x0001);
        C = lsb;
        value = static_cast<Word>((lsb << 15) | (temp >> 1));
        N = is_negative(value);
        Z = is_zero(value);
        V = N ^ C;
    } break;

    case ROL: {
        uint16_t temp = static_cast<uint16_t>(value);
        uint16_t msb = (temp & 0x8000) >> 15;
        C = msb;
        value = static_cast<Word>((temp << 1) | msb);
        N = is_negative(value);
        Z = is_zero(value);
        V = N ^ C;
    } break;

    case ASR: {
        uint16_t temp = static_cast<uint16_t>(value);
        uint16_t lsb = (temp & 0x0001);
        uint16_t msb = (temp & 0x8000);
        C = lsb;
        value = static_cast<Word>(msb | (temp >> 1));
        N = is_negative(value);
        Z = is_zero(value);
        V = N ^ C;
    } break;

    case ASL: {
        uint16_t temp = static_cast<uint16_t>(value);
        uint16_t msb = (temp & 0x8000) >> 15;
        C = msb;
        value = static_cast<Word>(temp << 1);
        N = is_negative(value);
        Z = is_zero(value);
        V = N ^ C;
    } break;

    case ADC: {
        V = is_overflow(value, C, value + C);
        int temp = C;
        C = is_carry(value, temp, Alu::Plus);
        value += temp;
        N = is_negative(value);
        Z = is_zero(value);
    } break;

    case SBC: {
        V = is_overflow(value, C, value - C);
        int temp = C;
        C = is_carry(value, temp, Alu::Minus);
        value -= temp;
        N = value < 0;
        Z = value == 0;
    } break;

    default:
        break;
    }

    return value;
}

void Alu::ccc(Byte byte) { NZVC &= ~(byte & 0x0F); }

void Alu::scc(Byte byte) { NZVC |= (byte & 0x0F); }

void Alu::jmp(Byte next_byte) {
    //// TODO Testar JMP
    std::size_t mmm = (next_byte & 0b00111000) >> 3;
    std::size_t rrr = (next_byte & 0b00000111);
    AddressMode mode = INT_TO_ADDRESSMODE[mmm];
    std::size_t address = cpu->get_absolute_address(mode, rrr) - MEM_SIZE;
    PC = address;
}

void Alu::sob([[maybe_unused]] Word word) {}

void Alu::jsr([[maybe_unused]] Word word) {}

void Alu::rts([[maybe_unused]] Byte byte) {}

/*
Word Alu::mov(Word src, Word dst) {
}

Word Alu::add(Word src, Word dst) {

}

Word Alu::sub(Word src, Word dst) {

}

Word Alu::cmp(Word src, Word dst) {

}

Word Alu::bitwise_and(Word src, Word dst) {

}

Word Alu::bitwise_or(Word src, Word dst) {

}
*/

Word Alu::two_operand_instruction(Instruction instruction, Word src, Word dst) {
    switch (instruction) {
    case MOV:
        return mov(src, dst);
    case ADD:
        return add(src, dst);
    case SUB:
        return sub(src, dst);
    case CMP:
        return cmp(src, dst);
    case AND:
        return bitwise_and(src, dst);
    case OR:
        return bitwise_or(src, dst);
    default:
        return dst; // NOP
    }
}

} // namespace hardware
