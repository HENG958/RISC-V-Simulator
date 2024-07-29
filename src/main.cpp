#include <bits/stdc++.h>

#include "memory.hpp"
#include "parser.hpp"

Memory memory;

int main() {
    //freopen("testcases/array_test1.data", "r", stdin);
    //freopen("test.out", "w", stdout);
    int cnt = 0;
    memory.Read();
    memory.ResetPC();
    while (true) {
        cnt++;
        //std::cout << cnt << std::endl;
        unsigned code = memory.GetInstruction();
        if (code == 0x0ff00513) {
            std::cout << static_cast<unsigned>(memory.Return() & 255u)
                      << std::endl;
            return 0;
        }
        Operation op = Parse(code);
        switch (op.op) {
            case Operation::LUI:
                memory.Lui(op);
                break;
            case Operation::AUIPC:
                memory.Auipc(op);
                break;
            case Operation::JAL:
                memory.Jal(op);
                break;
            case Operation::JALR:
                memory.Jalr(op);
                break;
            case Operation::BEQ:
                memory.Beq(op);
                break;
            case Operation::BNE:
                memory.Bne(op);
                break;
            case Operation::BLT:
                memory.Blt(op);
                break;
            case Operation::BGE:
                memory.Bge(op);
                break;
            case Operation::BLTU:
                memory.Bltu(op);
                break;
            case Operation::BGEU:
                memory.Bgeu(op);
                break;
            case Operation::LB:
                memory.Lb(op);
                break;
            case Operation::LH:
                memory.Lh(op);
                break;
            case Operation::LW:
                memory.Lw(op);
                break;
            case Operation::LBU:
                memory.Lbu(op);
                break;
            case Operation::LHU:
                memory.Lhu(op);
                break;
            case Operation::SB:
                memory.Sb(op);
                break;
            case Operation::SH:
                memory.Sh(op);
                break;
            case Operation::SW:
                memory.Sw(op);
                break;
            case Operation::ADDI:
                memory.Addi(op);
                break;
            case Operation::SLTI:
                memory.Slti(op);
                break;
            case Operation::SLTIU:
                memory.Sltiu(op);
                break;
            case Operation::XORI:
                memory.Xori(op);
                break;
            case Operation::ORI:
                memory.Ori(op);
                break;
            case Operation::ANDI:
                memory.Andi(op);
                break;
            case Operation::SLLI:
                memory.Slli(op);
                break;
            case Operation::SRLI:
                memory.Srli(op);
                break;
            case Operation::SRAI:
                memory.Srai(op);
                break;
            case Operation::ADD:
                memory.Add(op);
                break;
            case Operation::SUB:
                memory.Sub(op);
                break;
            case Operation::SLL:
                memory.Sll(op);
                break;
            case Operation::SLT:
                memory.Slt(op);
                break;
            case Operation::SLTU:
                memory.Sltu(op);
                break;
            case Operation::XOR:
                memory.Xor(op);
                break;
            case Operation::SRL:
                memory.Srl(op);
                break;
            case Operation::SRA:
                memory.Sra(op);
                break;
            case Operation::OR:
                memory.Or(op);
                break;
            case Operation::AND:
                memory.And(op);
                break;
        }
        memory.ResetRes();
    }
    return 0;
}