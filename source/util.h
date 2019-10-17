#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <bitset>
#include "constants.h"

inline void print_binary_byte(const Byte &byte) {
    std::cout << std::bitset<8>(byte) << '\n';
}

#endif
