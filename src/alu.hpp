#ifndef ALU_H
#define ALU_H
#include "operation.hpp"
class Alu {
   public:
    int Work(Operation::Oper op, int arg1, int arg2, int imm, int shamt);
   private:
    int Addi(int arg1, int imm);
    int Slti(int arg1, int imm);
    int Sltiu(int arg1, int imm);
    int Andi(int arg1, int imm);
    int Ori(int arg1, int imm);
    int Xori(int arg1, int imm);
    int Slli(int arg1, int shamt);
    int Srli(int arg1, int shamt);
    int Srai(int arg1, int shamt);
    int Add(int arg1, int arg2);
    int Slt(int arg1, int arg2);
    int Sltu(int arg1, int arg2);
    int And(int arg1, int arg2);
    int Or(int arg1, int arg2);
    int Xor(int arg1, int arg2);
    int Sll(int arg1, int arg2);
    int Srl(int arg1, int arg2);
    int Sub(int arg1, int arg2);
    int Sra(int arg1, int arg2);
    int Beq(int arg1,int arg2);
    int Bne(int arg1,int arg2);
    int Blt(int arg1,int arg2);
    int Bge(int arg1,int arg2);
    int Bltu(int arg1,int arg2);
    int Bgeu(int arg1,int arg2);
};
#endif