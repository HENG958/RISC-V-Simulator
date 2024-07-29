#include "memory.hpp"

#include <cstring>
#include <iostream>

int Memory::CharToInt(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else
        return ch - 'A' + 10;
}

void Memory::Read() {
    char str[20];
    std::cin >> str;
    while (!std::cin.eof()) {
        if (str[0] == '@') {
            now_pc = 0;
            int len = strlen(str);
            for (int i = 1; i < len; i++)
                now_pc = now_pc * 16 + CharToInt(str[i]);
        } else
            mem[now_pc++] = CharToInt(str[0]) * 16 + CharToInt(str[1]);
        std::cin >> str;
    }
}

int Memory::Return() { return reg[10]; }

void Memory::ResetPC() { now_pc = 0; }

unsigned Memory::GetInstruction() {
    unsigned ret = 0;
    for (int i = now_pc + 3; i >= now_pc; i--)
        ret = ret << 8 | static_cast<unsigned>(mem[i]);
    return ret;
}

void Memory::Addi(const Operation &op) {
    reg[op.rd] = reg[op.rs1] + op.imm;
    now_pc += 4;
}

void Memory::Slti(const Operation &op) {
    reg[op.rd] = reg[op.rs1] < op.imm;
    now_pc += 4;
}

void Memory::Sltiu(const Operation &op) {
    reg[op.rd] =
        static_cast<unsigned>(reg[op.rs1]) < static_cast<unsigned>(op.imm);
    now_pc += 4;
}

void Memory::Andi(const Operation &op) {
    reg[op.rd] = reg[op.rs1] & op.imm;
    now_pc += 4;
}

void Memory::Ori(const Operation &op) {
    reg[op.rd] = reg[op.rs1] | op.imm;
    now_pc += 4;
}

void Memory::Xori(const Operation &op) {
    reg[op.rd] = reg[op.rs1] ^ op.imm;
    now_pc += 4;
}

void Memory::Slli(const Operation &op) {
    reg[op.rd] = reg[op.rs1] << op.shamt;
    now_pc += 4;
}

void Memory::Srli(const Operation &op) {
    reg[op.rd] = static_cast<unsigned>(reg[op.rs1]) >> op.shamt;
    now_pc += 4;
}

void Memory::Srai(const Operation &op) {
    //算数位移
    reg[op.rd] = reg[op.rs1] >> op.shamt;
    now_pc += 4;
}

void Memory::Lui(const Operation &op) {
    reg[op.rd] = op.imm;
    now_pc += 4;
}

void Memory::Auipc(const Operation &op) {
    now_pc += op.imm;
    reg[op.rd] = now_pc;
    now_pc += 4;
}

void Memory::Add(const Operation &op) {
    reg[op.rd] = reg[op.rs1] + reg[op.rs2];
    now_pc += 4;
}

void Memory::Slt(const Operation &op) {
    reg[op.rd] = reg[op.rs1] < reg[op.rs2];
    now_pc += 4;
}

void Memory::Sltu(const Operation &op) {
    reg[op.rd] =
        static_cast<unsigned>(reg[op.rs1]) < static_cast<unsigned>(reg[op.rs2]);
    now_pc += 4;
}

void Memory::And(const Operation &op) {
    reg[op.rd] = reg[op.rs1] & reg[op.rs2];
    now_pc += 4;
}

void Memory::Or(const Operation &op) {
    reg[op.rd] = reg[op.rs1] | reg[op.rs2];
    now_pc += 4;
}

void Memory::Xor(const Operation &op) {
    reg[op.rd] = reg[op.rs1] ^ reg[op.rs2];
    now_pc += 4;
}

void Memory::Sll(const Operation &op) {
    reg[op.rd] = reg[op.rs1] << reg[op.rs2];
    now_pc += 4;
}

void Memory::Srl(const Operation &op) {
    reg[op.rd] = static_cast<unsigned>(reg[op.rs1]) >> reg[op.rs2];
    now_pc += 4;
}

void Memory::Sra(const Operation &op) {
    reg[op.rd] = reg[op.rs1] >> reg[op.rs2];
    now_pc += 4;
}

void Memory::Jal(const Operation &op) {
    reg[op.rd] = now_pc + 4;
    now_pc += op.imm;
}

void Memory::Jalr(const Operation &op) {
    //
    int tmp = now_pc + 4;
    now_pc = (reg[op.rs1] + op.imm) & ~1;
    reg[op.rd] = tmp;
}

void Memory::Beq(const Operation &op) {
    if (reg[op.rs1] == reg[op.rs2])
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Bne(const Operation &op) {
    if (reg[op.rs1] != reg[op.rs2])
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Blt(const Operation &op) {
    if (reg[op.rs1] < reg[op.rs2])
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Bge(const Operation &op) {
    if (reg[op.rs1] >= reg[op.rs2])
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Bltu(const Operation &op) {
    if (static_cast<unsigned>(reg[op.rs1]) < static_cast<unsigned>(reg[op.rs2]))
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Bgeu(const Operation &op) {
    if (static_cast<unsigned>(reg[op.rs1]) >=
        static_cast<unsigned>(reg[op.rs2]))
        now_pc += op.imm;
    else
        now_pc += 4;
}

void Memory::Lb(const Operation &op) {
    if (mem[reg[op.rs1] + op.imm] >> 7)
        reg[op.rd] = (~0b11111111) | mem[reg[op.rs1] + op.imm];
    else
        reg[op.rd] = mem[reg[op.rs1] + op.imm];
    now_pc += 4;
}

void Memory::Lh(const Operation &op) {
    unsigned read = 0;
    for (int i = 1; i >= 0; i--)
        read = (read << 8) | mem[reg[op.rs1] + op.imm + i];
    if (read >> 15)
        reg[op.rd] = (~0b1111111111111111) | read;
    else
        reg[op.rd] = read;
    now_pc += 4;
}

void Memory::Lw(const Operation &op) {
    unsigned read = 0;
    for (int i = 3; i >= 0; i--)
        read = (read << 8) | mem[reg[op.rs1] + op.imm + i];
    reg[op.rd] = read;
    now_pc += 4;
}

void Memory::Lbu(const Operation &op) {
    reg[op.rd] = mem[reg[op.rs1] + op.imm];
    now_pc += 4;
}

void Memory::Lhu(const Operation &op) {
    unsigned read = 0;
    for (int i = 1; i >= 0; i--)
        read = (read << 8) | mem[reg[op.rs1] + op.imm + i];
    reg[op.rd] = read;
    now_pc += 4;
}

void Memory::Sb(const Operation &op) {
    mem[reg[op.rs1] + op.imm] = reg[op.rs2];
    now_pc += 4;
}

void Memory::Sh(const Operation &op) {
    unsigned tmp = reg[op.rs2];
    for (int i = 0; i <= 1; i++) {
        mem[reg[op.rs1] + op.imm + i] = tmp;
        tmp >>= 8;
    }
    now_pc += 4;
}

void Memory::Sw(const Operation &op) {
    unsigned tmp = reg[op.rs2];
    for (int i = 0; i <= 3; i++) {
        mem[reg[op.rs1] + op.imm + i] = tmp;
        tmp >>= 8;
    }
    now_pc += 4;
}

void Memory::Sub(const Operation &op) {
    reg[op.rd] = reg[op.rs1] - reg[op.rs2];
    now_pc += 4;
}