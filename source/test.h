#ifndef TEST_H
#define TEST_H

#include "hardware.h"
#include <cstdio>
#include <bitset>
#include <fstream>
#include <arpa/inet.h>
#include <stddef.h>
#include "constants.h"
#include <iostream>
#include "util.h"


/*
void test_ccc(Cpu &cpu) {
    cpu.condition.all = 0b1111;
    Byte ccc = 0b00011010;
    cpu.memory[0] = ccc;
    cpu.registers[7] = 0;
    cpu.execute_next_instruction();
    std::bitset<4> condition = cpu.condition.all;
    std::cout << "0101 = " << condition << "\n";
}

void test_scc(Cpu &cpu) {
    cpu.condition.all = 0b0000;
    Byte scc = 0b00101010;
    cpu.memory[0] = scc;
    cpu.registers[7] = 0;
    cpu.execute_next_instruction();
    std::bitset<4> condition = cpu.condition.all;
    std::cout << "1010 = " << condition << "\n";
}

void test_br(Cpu &cpu) {
    cpu.registers[7] = 0;
    cpu.memory[0] = 0b00110000; // br
    cpu.memory[1] = 14;
    cpu.execute_next_instruction();
    printf("2 + 14 = 16 -> %d\n", cpu.registers[7]);
    cpu.registers[7] = 0;
    cpu.memory[1] = 0b10000001; // -127
    cpu.execute_next_instruction();
    printf("2 - 127 = -125 -> %d %u %x\n", cpu.registers[7], 0xFFFF & cpu.registers[7], 0xFFFF & cpu.registers[7]);
}
*/

void run_tests() {
    //Cpu cpu;
    //cpu.set_register(0, 34);
    //std::cout << cpu.get_register(0) << '\n';
    //cpu.increment_register(0, 4);
    //std::cout << cpu.get_register(0) << '\n';

    //printf("%lu\n", offsetof(Cpu, memory));

    //Byte *p = (Byte *) &(cpu.addressable_memory);
    //*(p + 0) = 0x0f;
    //*(p + 1) = 0xf0;
    //printf("%x\n", 0xFFFF & cpu.registers[0]);

    std::string filename{"test.mem"};
    Cpu cpu;
    Byte *memory = cpu.get_memory();
    cpu.read_memory_from_binary_file(filename);
    std::size_t i = 0;

    while (i < MEM_SIZE) {
        std::cout << i << ": ";
        print_binary_byte(memory[i]);
        cpu.execute_next_instruction();
        std::cout << "\n";
        ++i;
    }
}

#endif
