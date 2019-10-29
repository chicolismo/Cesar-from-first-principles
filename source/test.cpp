#include "hardware.h"
#include <gtest/gtest.h>

Cpu cpu;
Alu &alu = *(cpu.alu);

TEST(Cpu, std::int8_tsTostd::int16_t) {
    std::int8_t a = 0x01;
    std::int8_t b = 0x10;
    std::int16_t y = 0x0110;
    std::int16_t w = cpu.bytes_to_word(a, b);
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

    cpu.condition.all = 0b0000;
    alu.scc(0b1000u);
    EXPECT_EQ(cpu.condition.negative, 1u);

    cpu.condition.all = 0b0000;
    alu.scc(0b0100u);
    EXPECT_EQ(cpu.condition.zero, 1u);

    cpu.condition.all = 0b0000;
    alu.scc(0b0010u);
    EXPECT_EQ(cpu.condition.overflow, 1u);

    cpu.condition.all = 0b0000;
    alu.scc(0b0001u);
    EXPECT_EQ(cpu.condition.carry, 1u);
}

TEST(Cpu, Ccc) {
    cpu.condition.all = 0b1111u;

    alu.ccc(0b1010u);
    EXPECT_EQ(cpu.condition.all, 0b0101u);

    alu.ccc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b0000u);
}

TEST(Cpu, Overflow) {
    std::int16_t a = 0b0111111111111111;
    std::int16_t b = 0b0000000000000001;
    EXPECT_TRUE(alu.is_overflow(a, b, a + b));

    a = 0b1111111111111111;
    b = 0b1000000000000000;
    EXPECT_TRUE(alu.is_overflow(a, b, a - b));
}

TEST(Cpu, Zero) {
    std::int16_t a = 0;
    EXPECT_TRUE(alu.is_zero(a));
    a = -1;
    EXPECT_FALSE(alu.is_zero(a));
    a = 1;
    EXPECT_FALSE(alu.is_zero(a));
}

TEST(Cpu, Negative) {
    std::int16_t a = -1;
    EXPECT_TRUE(alu.is_negative(a));
    a = 0;
    EXPECT_FALSE(alu.is_negative(a));
    a = 1;
    EXPECT_FALSE(alu.is_negative(a));
}

TEST(Cpu, Carry) {
    std::int16_t a = 0xFFFF;
    std::int16_t b = 1;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Plus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Minus));

    a = 0;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Minus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Plus));
}

TEST(Alu, Neg) {
    std::int16_t w = 1;
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

    EXPECT_EQ(cpu.registers[7], static_cast<std::int16_t>(2000 - Cpu::MEMORY_OFFSET));

    const std::int8_t jump_byte = 0b000001001;
    std::size_t mmm = (jump_byte & 0b00111000) >> 3;
    std::size_t rrr = (jump_byte & 0b00000111);
    mode = INT_TO_ADDRESSMODE[mmm];
    cpu.registers[1] = 5000;
    cpu.registers[7] = 0;
    address = cpu.get_absolute_address(mode, rrr);
    alu.jmp(mode, address);
    EXPECT_EQ(cpu.registers[7], static_cast<std::int16_t>(5000));
    EXPECT_EQ(cpu.registers[1], 5002);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
