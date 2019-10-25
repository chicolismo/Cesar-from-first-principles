#include "hardware.h"
#include <iostream>

#define SP cpu->registers[8]
#define PC cpu->registers[7]
#define NZVC cpu->condition.all
#define N cpu->condition.negative
#define Z cpu->condition.zero
#define V cpu->condition.overflow
#define C cpu->condition.carry


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


Word Alu::one_operand_instruction(const Instruction instruction, const Word value) {
    // TODO. Garantir que todos as condições foram testadas.

    Word result;
    switch (instruction) {
    case CLR:
        result = 0;
        N = 0;
        Z = 1;
        C = 0;
        V = 0;
        break;

    case NOT:
        result = ~(static_cast<UWord>(value));
        N = is_negative(result);
        Z = is_zero(result);
        C = 1;
        V = 0;
        break;

    case INC:
        result = value + 1;
        V = is_overflow(value, 1, value + 1);
        C = static_cast<UWord>(result) == 0xFFFFu;
        N = is_negative(result);
        Z = is_zero(result);
        break;

    case DEC:
        result = value - 1;
        V = is_overflow(value, 1, value - 1);
        C = value == 0;
        N = is_negative(result);
        Z = is_zero(result);
        break;

    case NEG: {
        result = -value;
        const auto uvalue = static_cast<UWord>(value);
        const uint32_t temp = ~uvalue + 1;
        V = is_overflow(~value, 1, (~value) + 1); // TODO: Verificar isto.
        C = (temp & 0x10000) > 0;
        N = is_negative(result);
        Z = is_zero(result);
    }
        break;

    case TST:
        result = value;
        N = is_negative(result);
        Z = is_zero(result);
        C = 0;
        V = 0;
        break;

    case ROR: {
        const auto temp = static_cast<UWord>(value);
        const auto lsb = (temp & 0x0001u);
        result = static_cast<Word>(lsb << 15u | temp >> 1u);
        C = lsb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    }
        break;

    case ROL: {
        const auto temp = static_cast<UWord>(value);
        const auto msb = static_cast<UWord>((temp & 0x8000u) >> 15u);
        result = static_cast<Word>((temp << 1u) | msb);
        C = msb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    }
        break;

    case ASR: {
        const auto temp = static_cast<UWord>(value);
        const auto lsb = (temp & 0x0001u);
        const auto msb = (temp & 0x8000u);
        result = static_cast<Word>(msb | (temp >> 1u));
        C = lsb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    }
        break;

    case ASL: {
        const auto temp = static_cast<UWord>(value);
        const auto msb = (temp & 0x8000u) >> 15u;
        result = static_cast<Word>(temp << 1u);
        C = msb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    }
        break;

    case ADC: {
        result = value + C;
        N = is_negative(result);
        Z = is_zero(result);
        V = is_overflow(value, C, value + C);
        C = is_carry(value, C, Alu::Plus);
    }
        break;

    case SBC: {
        result = value + C;
        N = is_negative(result);
        Z = is_zero(result);
        V = is_overflow(value, C, value - C);
        C = is_carry(value, C, Alu::Minus);
    }
        break;

    default:
        result = value;
        break;
    }

    return result;
}


void Alu::ccc(const Byte byte) { NZVC &= ~(static_cast<UByte>(byte) & 0x0Fu); }


void Alu::scc(const Byte byte) { NZVC |= (static_cast<UByte>(byte) & 0x0Fu); }


void Alu::jmp(const AddressMode mode, const std::size_t absolute_address) {
    if (mode != REGISTER) {
        auto address = absolute_address - Cpu::MEMORY_OFFSET;
        PC = address;
    }
}


void Alu::sob(const std::size_t register_number, const Byte offset) {
    if ((--cpu->registers[register_number]) != 0) {
        PC -= offset;
    }
}


void Alu::jsr(const AddressMode mode, const std::size_t sub_address, const int register_number) {
    if (mode != REGISTER) {
        auto temp = sub_address - Cpu::MEMORY_OFFSET;
        cpu->push(cpu->registers[register_number]);
        cpu->registers[register_number] = PC;
        PC = static_cast<Word>(temp);
    }
}


void Alu::rts(const Byte byte) {
    std::size_t rrr = (static_cast<UByte>(byte) & 0b00000111);
    PC = cpu->registers[rrr];
    cpu->registers[rrr] = cpu->pop();
}


Word Alu::mov(const Word src) {
    N = is_negative(src);
    Z = is_zero(src);
    V = 0;
    return src;
}


Word Alu::add(Word src, Word dst) {
    Word result = dst + src;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(dst, src, result);
    C = is_carry(dst, src, CarryOperation::Plus);
    return result;
}


Word Alu::sub(const Word src, const Word dst) {
    Word result = dst - src;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(dst, src, result);
    C = is_carry(dst, src, CarryOperation::Minus);
    return result;
}


Word Alu::cmp(const Word src, const Word dst) {
    Word result = src - dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(src, dst, result);
    C = is_carry(src, dst, CarryOperation::Minus);
    return dst;
}


Word Alu::bitwise_and(const Word src, const Word dst) {
    Word result = src & dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = 0;
    return result;
}


Word Alu::bitwise_or(const Word src, const Word dst) {
    Word result = src | dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = 0;
    return result;
}


Word Alu::two_operand_instruction(const Instruction instruction, const Word src, const Word dst) {

    switch (instruction) {
    case MOV:
        return mov(src);
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
        std::cerr << __FILE__ << ':' << __LINE__ << " Código não pode chegar aqui!\n";
        std::exit(1);
    }
}
