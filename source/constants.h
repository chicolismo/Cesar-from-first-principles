#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <map>
#include <string>

constexpr std::size_t MEM_SIZE = 1 << 16;

using Byte = std::int8_t;
using Word = std::int16_t;

enum AddressMode {
    REGISTER                  = 0,
    REGISTER_POST_INCREMENTED = 1,
    REGISTER_PRE_DECREMENTED  = 2,
    INDEXED                   = 3,
    REGISTER_INDIRECT         = 4,
    POST_INCREMENTED_INDIRECT = 5,
    PRE_DECREMENTED_INDIRECT  = 6,
    INDEXED_INDIRECT          = 7
};

constexpr AddressMode INT_TO_ADDRESSMODE[8] {
    REGISTER,
    REGISTER_POST_INCREMENTED,
    REGISTER_PRE_DECREMENTED,
    INDEXED,
    REGISTER_INDIRECT,
    POST_INCREMENTED_INDIRECT,
    PRE_DECREMENTED_INDIRECT,
    INDEXED_INDIRECT
};

enum Instruction {
    NOP, CCC, SCC, BR, BNE, BEQ, BPL, BMI, BVC, BVS, BCC, BCS, BGE, BLT, BGT,
    BLE, BHI, BLS, JMP, SOB, JSR, RTS, CLR, NOT, INC, DEC, NEG, TST, ROR, ROL,
    ASR, ASL, ADC, SBC, MOV, ADD, SUB, CMP, AND, OR, HLT, CONDITIONAL_BRANCH,
    ONE_OPERAND_INSTRUCTION
};

const std::map<Byte, Instruction> OPCODE_TO_INSTRUCTION = {
    {0b0000, NOP},     {0b0001, CCC},
    {0b0010, SCC},     {0b0011, CONDITIONAL_BRANCH},
    {0b00110000, BR},  {0b00110001, BNE},
    {0b00110010, BEQ}, {0b00110011, BPL},
    {0b00110100, BMI}, {0b00110101, BVC},
    {0b00110110, BVS}, {0b00110111, BCC},
    {0b00111000, BCS}, {0b00111001, BGE},
    {0b00111010, BLT}, {0b00111011, BGT},
    {0b00111100, BLE}, {0b00111101, BHI},
    {0b00111110, BLS}, {0b0100, JMP},
    {0b0101, SOB},     {0b0110, JSR},
    {0b0111, RTS},     {0b1000, ONE_OPERAND_INSTRUCTION},
    {0b10000000, CLR}, {0b10000001, NOT},
    {0b10000010, INC}, {0b10000011, DEC},
    {0b10000100, NEG}, {0b10000101, TST},
    {0b10000110, ROR}, {0b10000111, ROL},
    {0b10001000, ASR}, {0b10001001, ASL},
    {0b10001010, ADC}, {0b10001011, SBC},
    {0b1001, MOV},     {0b1010, ADD},
    {0b1011, SUB},     {0b1100, CMP},
    {0b1101, AND},     {0b1110, OR},
    {0b1111, HLT}
};

constexpr std::string_view INSTRUCTION_NAMES[] = {
    [NOP] = "NOP",
    [CCC] = "CCC",
    [SCC] = "SCC",
    [BR] = "BR",
    [BNE] = "BNE",
    [BEQ] = "BEQ",
    [BPL] = "BPL",
    [BMI] = "BMI",
    [BVC] = "BVC",
    [BVS] = "BVS",
    [BCC] = "BCC",
    [BCS] = "BCS",
    [BGE] = "BGE",
    [BLT] = "BLT",
    [BGT] = "BGT",

    [BLE] = "BLE",
    [BHI] = "BHI",
    [BLS] = "BLS",
    [JMP] = "JMP",
    [SOB] = "SOB",
    [JSR] = "JSR",
    [RTS] = "RTS",
    [CLR] = "CLR",
    [NOT] = "NOT",
    [INC] = "INC",
    [DEC] = "DEC",
    [NEG] = "NEG",
    [TST] = "TST",
    [ROR] = "ROR",
    [ROL] = "ROL",

    [ASR] = "ASR",
    [ASL] = "ASL",
    [ADC] = "ADC",
    [SBC] = "SBC",
    [MOV] = "MOV",
    [ADD] = "ADD",
    [SUB] = "SUB",
    [CMP] = "CMP",
    [AND] = "AND",
    [OR] = "OR",
    [HLT] = "HLT",
    [CONDITIONAL_BRANCH] = "Conditional Branch",
    [ONE_OPERAND_INSTRUCTION] = "One operand instruction",
};

#endif
