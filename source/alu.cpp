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


void Alu::conditional_branch(Instruction instruction, std::int8_t offset) {
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


std::int16_t Alu::one_operand_instruction(const Instruction instruction, const std::int16_t value) {
    // TODO. Garantir que todos as condições foram testadas.

    std::int16_t result;
    switch (instruction) {
    case CLR:
        result = 0;
        N = 0;
        Z = 1;
        V = 0;
        C = 0;
        break;

    case NOT:
        result = ~(static_cast<std::uint16_t>(value));
        N = is_negative(result);
        Z = is_zero(result);
        V = 0;
        C = 1;
        break;

    case INC:
        result = value + 1;
        V = is_overflow(value, 1, value + 1);
        C = static_cast<std::uint16_t>(value) == 0xFFFFu;
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
        const auto uvalue = static_cast<std::uint16_t>(value);
        const uint32_t temp = ~uvalue + 1;
        V = is_overflow(~value, 1, (~value) + 1); // TODO: Verificar isto.
        C = (temp & 0x10000) > 0;
        N = is_negative(result);
        Z = is_zero(result);
    } break;

    case TST:
        result = value;
        N = is_negative(result);
        Z = is_zero(result);
        V = 0;
        C = 0;
        break;

    case ROR: {
        const auto temp = static_cast<std::uint16_t>(value);
        const auto lsb = (temp & 0x0001u);
        result = static_cast<std::int16_t>(lsb << 15u | temp >> 1u);
        C = lsb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    } break;

    case ROL: {
        const auto temp = static_cast<std::uint16_t>(value);
        const auto msb = static_cast<std::uint16_t>((temp & 0x8000u) >> 15u);
        result = static_cast<std::int16_t>((temp << 1u) | msb);
        C = msb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    } break;

    case ASR: {
        const auto temp = static_cast<std::uint16_t>(value);
        const auto lsb = (temp & 0x0001u);
        const auto msb = (temp & 0x8000u);
        result = static_cast<std::int16_t>(msb | (temp >> 1u));
        C = lsb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    } break;

    case ASL: {
        const auto temp = static_cast<std::uint16_t>(value);
        const auto msb = (temp & 0x8000u) >> 15u;
        result = static_cast<std::int16_t>(temp << 1u);
        C = msb;
        N = is_negative(result);
        Z = is_zero(result);
        V = N ^ C;
    } break;

    case ADC: {
        result = value + C;
        N = is_negative(result);
        Z = is_zero(result);
        V = is_overflow(value, C, value + C);
        C = is_carry(value, C, Alu::Plus);
    } break;

    case SBC: {
        result = value - C;
        N = is_negative(result);
        Z = is_zero(result);
        V = is_overflow(value, C, value - C);
        C = is_carry(value, C, Alu::Minus);
    } break;

    default:
        result = value;
        break;
    }

    return result;
}


void Alu::ccc(const std::int8_t byte) {
    NZVC &= ~(static_cast<std::uint8_t>(byte) & 0x0Fu);
}


void Alu::scc(const std::int8_t byte) {
    NZVC |= (static_cast<std::uint8_t>(byte) & 0x0Fu);
}


void Alu::jmp(const AddressMode mode, const std::size_t absolute_address) {
    if (mode != REGISTER) {
        auto address = absolute_address - Cpu::MEMORY_OFFSET;
        PC = address;
    }
}


void Alu::sob(const std::size_t register_number, const std::int8_t offset) {
    if ((--cpu->registers[register_number]) != 0) {
        PC -= offset;
    }
}


void Alu::jsr(const AddressMode mode, const std::size_t sub_address, const int register_number) {
    if (mode != REGISTER) {
        auto temp = sub_address - Cpu::MEMORY_OFFSET;
        cpu->push(cpu->registers[register_number]);
        cpu->registers[register_number] = PC;
        PC = static_cast<std::int16_t>(temp);
    }
}


void Alu::rts(const std::int8_t byte) {
    int rrr = (static_cast<std::uint8_t>(byte) & 0b00000111);
    PC = cpu->registers[rrr];
    cpu->registers[rrr] = cpu->pop();
}


std::int16_t Alu::mov(const std::int16_t src) {
    N = is_negative(src);
    Z = is_zero(src);
    V = 0;
    return src;
}


std::int16_t Alu::add(std::int16_t src, std::int16_t dst) {
    std::int16_t result = dst + src;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(dst, src, result);
    C = is_carry(dst, src, CarryOperation::Plus);
    return result;
}


std::int16_t Alu::sub(const std::int16_t src, const std::int16_t dst) {
    std::int16_t result = dst - src;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(dst, src, result);
    C = is_carry(dst, src, CarryOperation::Minus);
    return result;
}


std::int16_t Alu::cmp(const std::int16_t src, const std::int16_t dst) {
    std::int16_t result = src - dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = is_overflow(src, dst, result);
    C = is_carry(src, dst, CarryOperation::Minus);
    return dst;
}


std::int16_t Alu::bitwise_and(const std::int16_t src, const std::int16_t dst) {
    std::int16_t result = src & dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = 0;
    return result;
}


std::int16_t Alu::bitwise_or(const std::int16_t src, const std::int16_t dst) {
    std::int16_t result = src | dst;
    N = is_negative(result);
    Z = is_zero(result);
    V = 0;
    return result;
}


std::int16_t Alu::two_operand_instruction(const Instruction instruction, const std::int16_t src, const std::int16_t dst) {

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
