#ifndef MEMORY_H
#define MEMORY_H
#include "operation.hpp"
struct Memory {
    unsigned char mem[500000];
    int pc;

    void Read();
    int CharToInt(char ch);
    int Load(Operation::Oper op, int addr);
    void Store(Operation::Oper op, int addr, int value);
    unsigned GetInstruction();
};

#endif