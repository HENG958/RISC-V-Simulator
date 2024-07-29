#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "operation.hpp"
class Memory {
   private:
    int reg[32] = {0};
    unsigned char mem[500000];
    unsigned tmp[3];
    int now_pc;

   public:
    int Return();
    void ResetRes() { reg[0] = 0; }
    void Read();
    int CharToInt(char ch);
    unsigned GetInstruction();
    void ResetPC();
    void Addi(const Operation &op);
    void Slti(const Operation &op);
    void Sltiu(const Operation &op);
    void Andi(const Operation &op);
    void Ori(const Operation &op);
    void Xori(const Operation &op);
    void Slli(const Operation &op);
    void Srli(const Operation &op);
    void Srai(const Operation &op);
    void Lui(const Operation &op);
    void Auipc(const Operation &op);
    void Add(const Operation &op);
    void Slt(const Operation &op);
    void Sltu(const Operation &op);
    void And(const Operation &op);
    void Or(const Operation &op);
    void Xor(const Operation &op);
    void Sll(const Operation &op);
    void Srl(const Operation &op);
    void Sub(const Operation &op);
    void Sra(const Operation &op);
    void Jal(const Operation &op);
    void Jalr(const Operation &op);
    void Beq(const Operation &op);
    void Bne(const Operation &op);
    void Blt(const Operation &op);
    void Bge(const Operation &op);
    void Bltu(const Operation &op);
    void Bgeu(const Operation &op);
    void Lw(const Operation &op);
    void Lh(const Operation &op);
    void Lhu(const Operation &op);
    void Lb(const Operation &op);
    void Lbu(const Operation &op);
    void Sw(const Operation &op);
    void Sh(const Operation &op);
    void Sb(const Operation &op);
};
#endif