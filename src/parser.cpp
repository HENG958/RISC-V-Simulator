#include "parser.hpp"
int GetImm(unsigned int bin_code, Operation::ImmType imm_type) {
    int ret = 0;
    int op = bin_code >> 31;
    if (imm_type == Operation::R) {
    } else if (imm_type == Operation::I) {
        if (op) {
            ret = 1;
            for (int i = 1; i < 20; i++) ret = ret << 1 | 1;
            ret <<= 12;
        }
        ret |= bin_code >> 20;
    } else if (imm_type == Operation::S) {
        if (op) {
            ret = 1;
            for (int i = 1; i < 20; i++) ret = ret << 1 | 1;
            ret <<= 7;
        }
        ret |= bin_code >> 25;
        ret = (ret << 5) | (bin_code >> 7 & 0b11111);
    } else if (imm_type == Operation::B) {
        if (op) {
            ret = 1;
            for (int i = 1; i < 19; i++) ret = ret << 1 | 1;
            ret <<= 1;
        }
        ret |= bin_code >> 31;
        ret = (ret << 1) | (bin_code >> 7 & 1);
        ret = (ret << 6) | (bin_code >> 25 & 0b111111);
        ret = (ret << 4) | (bin_code >> 8 & 0b1111);
        ret <<= 1;
    } else if (imm_type == Operation::U) {
        ret = (bin_code >> 12) << 12;
    } else if (imm_type == Operation::J) {
        if (op) {
            ret = 1;
            for (int i = 1; i < 11; i++) ret = ret << 1 | 1;
            ret <<= 1;
        }
        ret |= bin_code >> 31;
        ret = (ret << 8) | (bin_code >> 12 & 0b11111111);
        ret = (ret << 1) | (bin_code >> 20 & 1);
        ret = (ret << 10) | (bin_code >> 21 & 0b1111111111);
        ret <<= 1;
    }
    return ret;
}
int GetRs1(unsigned bin_code) { return bin_code >> 15 & 0b11111; }
int GetRs2(unsigned bin_code) { return bin_code >> 20 & 0b11111; }
int GetRd(unsigned bin_code) { return bin_code >> 7 & 0b11111; }
int GetFunct3(unsigned bin_code) { return bin_code >> 12 & 0b111; }
int GetFunct7(unsigned bin_code) { return bin_code >> 25; }
int GetShamt(unsigned bin_code) { return bin_code >> 20 & 0b11111; }
Operation Parse(unsigned bin_code) {
    Operation ret;
    int op = bin_code & 0b1111111;
    if (op == 0b0110111) {
        // LUI
        ret.op = Operation::LUI;
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::U);
    } else if (op == 0b0010111) {
        // AUIPC
        ret.op = Operation::AUIPC;
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::U);
    } else if (op == 0b1101111) {
        // JAL
        ret.op = Operation::JAL;
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::J);
    } else if (op == 0b1100111) {
        // JALR
        ret.op = Operation::JALR;
        ret.rs1 = GetRs1(bin_code);
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::I);
    } else if (op == 0b1100011) {
        // BEQ BNE BLT BGE BLTU BGEU
        ret.rs1 = GetRs1(bin_code);
        ret.rs2 = GetRs2(bin_code);
        ret.imm = GetImm(bin_code, Operation::B);
        int funct3 = GetFunct3(bin_code);
        if (funct3 == 0b000)
            ret.op = Operation::BEQ;
        else if (funct3 == 0b001)
            ret.op = Operation::BNE;
        else if (funct3 == 0b100)
            ret.op = Operation::BLT;
        else if (funct3 == 0b101)
            ret.op = Operation::BGE;
        else if (funct3 == 0b110)
            ret.op = Operation::BLTU;
        else if (funct3 == 0b111)
            ret.op = Operation::BGEU;
    } else if (op == 0b0000011) {
        // LB LH LW LBU LHU
        ret.rs1 = GetRs1(bin_code);
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::I);
        int funct3 = GetFunct3(bin_code);
        if (funct3 == 0b000)
            ret.op = Operation::LB;
        else if (funct3 == 0b001)
            ret.op = Operation::LH;
        else if (funct3 == 0b010)
            ret.op = Operation::LW;
        else if (funct3 == 0b100)
            ret.op = Operation::LBU;
        else if (funct3 == 0b101)
            ret.op = Operation::LHU;
    } else if (op == 0b0100011) {
        // SB SH SW
        ret.rs1 = GetRs1(bin_code);
        ret.rs2 = GetRs2(bin_code);
        ret.imm = GetImm(bin_code, Operation::S);
        int funct3 = GetFunct3(bin_code);
        if (funct3 == 0b000)
            ret.op = Operation::SB;
        else if (funct3 == 0b001)
            ret.op = Operation::SH;
        else if (funct3 == 0b010)
            ret.op = Operation::SW;
    } else if (op == 0b0010011) {
        // ADDI SLTI SLTIU XORI ORI ANDI
        ret.rs1 = GetRs1(bin_code);
        ret.rd = GetRd(bin_code);
        ret.imm = GetImm(bin_code, Operation::I);
        int funct3 = GetFunct3(bin_code);
        if (funct3 == 0b000)
            ret.op = Operation::ADDI;
        else if (funct3 == 0b010)
            ret.op = Operation::SLTI;
        else if (funct3 == 0b011)
            ret.op = Operation::SLTIU;
        else if (funct3 == 0b100)
            ret.op = Operation::XORI;
        else if (funct3 == 0b110)
            ret.op = Operation::ORI;
        else if (funct3 == 0b111)
            ret.op = Operation::ANDI;
        else if (funct3 == 0b001) {
            ret.op = Operation::SLLI;
            ret.shamt = GetShamt(bin_code);
        } else if (funct3 == 0b101) {
            int funct7 = GetFunct7(bin_code);
            if (funct7 == 0b0000000)
                ret.op = Operation::SRLI;
            else if (funct7 == 0b0100000)
                ret.op = Operation::SRAI;
            ret.shamt = GetShamt(bin_code);
        }
    } else if (op == 0b0110011) {
        ret.rs1 = GetRs1(bin_code);
        ret.rs2 = GetRs2(bin_code);
        ret.rd = GetRd(bin_code);
        int funct3 = GetFunct3(bin_code);
        int funct7 = GetFunct7(bin_code);
        if (funct3 == 0b000) {
            if (funct7 == 0b0000000)
                ret.op = Operation::ADD;
            else if (funct7 == 0b0100000)
                ret.op = Operation::SUB;
        } else if (funct3 == 0b001)
            ret.op = Operation::SLL;
        else if (funct3 == 0b010)
            ret.op = Operation::SLT;
        else if (funct3 == 0b011)
            ret.op = Operation::SLTU;
        else if (funct3 == 0b100)
            ret.op = Operation::XOR;
        else if (funct3 == 0b101) {
            if (funct7 == 0b0000000)
                ret.op = Operation::SRL;
            else if (funct7 == 0b0100000)
                ret.op = Operation::SRA;
        } else if (funct3 == 0b110)
            ret.op = Operation::OR;
        else if (funct3 == 0b111)
            ret.op = Operation::AND;
    }
    return ret;
}