#include "hardware.h"
#include <gtest/gtest.h>

using namespace hardware;

Cpu cpu;
Alu &alu = *(cpu.alu);

TEST(Cpu, BytesToWord) {
    Byte a = 0x01;
    Byte b = 0x10;
    Word y = 0x0110;
    Word w = cpu.bytes_to_word(a, b);
    EXPECT_EQ(w, y);

    a = 0xFF;
    b = 0xaa;
    y = 0xFFaa;
    w = cpu.bytes_to_word(a, b);
    EXPECT_EQ(w, y);
}

TEST(Cpu, Scc) {
    cpu.condition.all = 0b0000;
    alu.scc(0b1010u);
    EXPECT_EQ(cpu.condition.all, 0b1010u);
    alu.scc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b1111u);

    alu.scc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b1111u);
}

TEST(Cpu, Ccc) {
    cpu.condition.all = 0b1111u;

    alu.ccc(0b1010u);
    EXPECT_EQ(cpu.condition.all, 0b0101u);

    alu.ccc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b0000u);
}

TEST(Cpu, Overflow) {
    Word a = 0b0111111111111111;
    Word b = 0b0000000000000001;
    EXPECT_TRUE(alu.is_overflow(a, b, a + b));

    a = 0b1111111111111111;
    b = 0b1000000000000000;
    EXPECT_TRUE(alu.is_overflow(a, b, a - b));
}

TEST(Cpu, Zero) {
    Word a = 0;
    EXPECT_TRUE(alu.is_zero(a));
    a = -1;
    EXPECT_FALSE(alu.is_zero(a));
    a = 1;
    EXPECT_FALSE(alu.is_zero(a));
}

TEST(Cpu, Negative) {
    Word a = -1;
    EXPECT_TRUE(alu.is_negative(a));
    a = 0;
    EXPECT_FALSE(alu.is_negative(a));
    a = 1;
    EXPECT_FALSE(alu.is_negative(a));
}

TEST(Cpu, Carry) {
    Word a = 0xFFFF;
    Word b = 1;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Plus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Minus));

    a = 0;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Minus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Plus));
}

TEST(Alu, Neg) {
    Word w = 1;
    alu.ccc(0b1111u);
    alu.one_operand_instruction(NEG, w);
    EXPECT_EQ(cpu.condition.negative, 1u);
    EXPECT_EQ(cpu.condition.zero, 0u);
    EXPECT_EQ(cpu.condition.overflow, 0u);
    EXPECT_EQ(cpu.condition.carry, 1u);

    w = -1;
    alu.ccc(0b1111u);
    alu.one_operand_instruction(NEG, w);
    EXPECT_EQ(cpu.condition.negative, 0u);
    EXPECT_EQ(cpu.condition.zero, 0u);
    EXPECT_EQ(cpu.condition.overflow, 0u);
    EXPECT_EQ(cpu.condition.carry, 1u);

    w = 0;
    alu.ccc(0b1111u);
    alu.one_operand_instruction(NEG, w);
    EXPECT_EQ(cpu.condition.negative, 0u);
    EXPECT_EQ(cpu.condition.zero, 1u);
    EXPECT_EQ(cpu.condition.overflow, 0u);
    EXPECT_EQ(cpu.condition.carry, 0u);

    w = 0xFFFF;
    alu.ccc(0b1111u);
    alu.one_operand_instruction(NEG, w);
    EXPECT_EQ(cpu.condition.negative, 0u);
    EXPECT_EQ(cpu.condition.zero, 0u);
    EXPECT_EQ(cpu.condition.overflow, 0u);
    EXPECT_EQ(cpu.condition.carry, 1u);

    w = 0x7FFF;
    alu.ccc(0b1111u);
    alu.one_operand_instruction(NEG, w);
    EXPECT_EQ(cpu.condition.negative, 1u);
    EXPECT_EQ(cpu.condition.zero, 0u);
    EXPECT_EQ(cpu.condition.overflow, 0u);
    EXPECT_EQ(cpu.condition.carry, 1u);
}

TEST(Cpu, Jump) {
    cpu.registers[7] = 0;
    std::size_t address = 2000;
    AddressMode mode = INDEXED;
    alu.jmp(mode, address);

    EXPECT_EQ(cpu.registers[7], static_cast<Word>(2000 - Cpu::MEMORY_OFFSET));

    const Byte jump_byte = 0b000001001;
    std::size_t mmm = (jump_byte & 0b00111000) >> 3;
    std::size_t rrr = (jump_byte & 0b00000111);
    mode = INT_TO_ADDRESSMODE[mmm];
    cpu.registers[1] = 5000;
    cpu.registers[7] = 0;
    address = cpu.get_absolute_address(mode, rrr);
    alu.jmp(mode, address);
    EXPECT_EQ(cpu.registers[7], static_cast<Word>(5000));
    EXPECT_EQ(cpu.registers[1], 5002);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
