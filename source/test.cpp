#include "hardware.h"
#include <gtest/gtest.h>

using namespace hardware;

Cpu cpu;
Alu &alu = *(cpu.alu);

TEST(CpuTest, BytesToWord) {
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

TEST(CpuTest, Scc) {
    cpu.condition.all = 0b0000;
    alu.scc(0b1010u);
    EXPECT_EQ(cpu.condition.all, 0b1010u);
    alu.scc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b1111u);

    alu.scc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b1111u);
}

TEST(CpuTest, Ccc) {
    cpu.condition.all = 0b1111u;

    alu.ccc(0b1010u);
    EXPECT_EQ(cpu.condition.all, 0b0101u);

    alu.ccc(0b0101u);
    EXPECT_EQ(cpu.condition.all, 0b0000u);
}

TEST(CpuTest, Overflow) {
    Word a = 0b0111111111111111;
    Word b = 0b0000000000000001;
    EXPECT_TRUE(alu.is_overflow(a, b, a + b));

    a = 0b1111111111111111;
    b = 0b1000000000000000;
    EXPECT_TRUE(alu.is_overflow(a, b, a - b));
}

TEST(CpuTest, Zero) {
    Word a = 0;
    EXPECT_TRUE(alu.is_zero(a));
    a = -1;
    EXPECT_FALSE(alu.is_zero(a));
    a = 1;
    EXPECT_FALSE(alu.is_zero(a));
}

TEST(CpuTest, Negative) {
    Word a = -1;
    EXPECT_TRUE(alu.is_negative(a));
    a = 0;
    EXPECT_FALSE(alu.is_negative(a));
    a = 1;
    EXPECT_FALSE(alu.is_negative(a));
}

TEST(CpuTest, Carry) {
    Word a = 0xFFFF;
    Word b = 1;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Plus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Minus));

    a = 0;
    EXPECT_TRUE(alu.is_carry(a, b, Alu::Minus));
    EXPECT_FALSE(alu.is_carry(a, b, Alu::Plus));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
