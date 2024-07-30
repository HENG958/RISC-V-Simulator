#include "alu.hpp"
int Alu::Addi(int arg1, int imm) {
    return arg1 + imm;
}

int Alu::Slti(int arg1, int imm) {
    return arg1 < imm;
}

int Alu::Sltiu(int arg1, int imm) {
    return static_cast<unsigned>(arg1) < static_cast<unsigned>(imm);
}

int Alu::Andi(int arg1, int imm) {
    return arg1 & imm;
}

int Alu::Ori(int arg1, int imm) {
    return arg1 | imm;
}

int Alu::Xori(int arg1, int imm) {
    return arg1 ^ imm;
}

int Alu::Slli(int arg1, int shamt) {
    return arg1 << shamt;
}

int Alu::Srli(int arg1, int shamt) {
    return static_cast<unsigned>(arg1) >> shamt;
}

int Alu::Srai(int arg1, int shamt) {
    return arg1 >> shamt;
}

int Alu::Add(int arg1, int arg2) {
    return arg1 + arg2;
}

int Alu::Slt(int arg1, int arg2) {
    return arg1 < arg2;
}

int Alu::Sltu(int arg1, int arg2) {
    return static_cast<unsigned>(arg1) < static_cast<unsigned>(arg2);
}

int Alu::And(int arg1, int arg2) {
    return arg1 & arg2;
}

int Alu::Or(int arg1, int arg2) {
    return arg1 | arg2;
}

int Alu::Xor(int arg1, int arg2) {
    return arg1 ^ arg2;
}

int Alu::Sll(int arg1, int arg2) {
    return arg1 << arg2;
}

int Alu::Srl(int arg1, int arg2) {
    return static_cast<unsigned>(arg1) >> arg2;
}

int Alu::Sra(int arg1, int arg2) {
    return arg1 >> arg2;
}

int Alu::Sub(int arg1, int arg2) { 
    return arg1 - arg2; 
}

int Alu::Beq(int arg1, int arg2) {
    if (arg1 == arg2) {
        return 1;
    } else
        return 0;
}

int Alu::Bne(int arg1, int arg2) {
    if (arg1 != arg2) {
        return 1;
    } else
        return 0;
}

int Alu::Blt(int arg1, int arg2) {
    if (arg1 < arg2) {
        return 1;
    } else
        return 0;
}

int Alu::Bge(int arg1, int arg2) {
    if (arg1 >= arg2) {
        return 1;
    } else
        return 0;
}

int Alu::Bltu(int arg1, int arg2) {
    if (static_cast<unsigned>(arg1) < static_cast<unsigned>(arg2)) {
        return 1;
    } else
        return 0;
}

int Alu::Bgeu(int arg1, int arg2) {
    if (static_cast<unsigned>(arg1) >= static_cast<unsigned>(arg2)) {
        return 1;
    } else
        return 0;
}

int Alu::Work(Operation::Oper op, int arg1, int arg2, int imm, int shamt) {
    switch (op) {
        case Operation::ADDI:
            return Addi(arg1, imm);
        case Operation::SLTI:
            return Slti(arg1, imm);
        case Operation::SLTIU:
            return Sltiu(arg1, imm);
        case Operation::XORI:
            return Xori(arg1, imm);
        case Operation::ORI:
            return Ori(arg1, imm);
        case Operation::ANDI:
            return Andi(arg1, imm);
        case Operation::SLLI:
            return Slli(arg1, shamt);
        case Operation::SRLI:
            return Srli(arg1, shamt);
        case Operation::SRAI:
            return Srai(arg1, shamt);
        case Operation::ADD:
            return Add(arg1, arg2);
        case Operation::SUB:
            return Sub(arg1, arg2);
        case Operation::SLL:
            return Sll(arg1, arg2);
        case Operation::SLT:
            return Slt(arg1, arg2);
        case Operation::SLTU:
            return Sltu(arg1, arg2);
        case Operation::XOR:
            return Xor(arg1, arg2);
        case Operation::SRL:
            return Srl(arg1, arg2);
        case Operation::SRA:
            return Sra(arg1, arg2);
        case Operation::OR:
            return Or(arg1, arg2);
        case Operation::AND:
            return And(arg1, arg2);
        case Operation::BEQ:
            return Beq(arg1, arg2);
        case Operation::BNE:
            return Bne(arg1, arg2);
        case Operation::BLT:
            return Blt(arg1, arg2);
        case Operation::BGE:
            return Bge(arg1, arg2);
        case Operation::BLTU:
            return Bltu(arg1, arg2);
        case Operation::BGEU:
            return Bgeu(arg1, arg2);
        default:
            return 0;
    }
}