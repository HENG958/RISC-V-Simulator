#ifndef TOMASULO_H
#define TOMASULO_H
#include "alu.hpp"
#include "array.hpp"
#include "memory.hpp"
#include "operation.hpp"
#include "queue.hpp"
class Tomasulo {
   private:
    struct Rob {
        Operation::Oper op;
        bool ready = false;
        int dest;  // 目标寄存器
        int addr;  // 地址
        int pc;
        int value;
        int offset;
        unsigned code = 0;
        void Clear() {
            op = Operation::LUI;
            dest = value = addr = pc = -1;
            offset = 0;
            ready = false;
        }
    };
    struct RSL {
        Operation::Oper op;
        int vj, vk, qj, qk;
        int dest;
        int imm;
        int shamt;
        int current_pc;  // 对JAL与JALR指令，记录当前位置
        void Clear() {
            vj = vk = qj = qk = -1;
            dest = imm = shamt = -1;
            current_pc = 0;
        }
    };
    struct RegStatus {
        int reorder;
        bool busy = false;
    };
    Memory memory;
    Alu alu;
    Queue<Rob> rob;
    Queue<RSL> ls;
    Array<RSL> rs;
    RegStatus reg_stat[32];
    int reg_in[32], reg_out[32];
    bool Issue();
    void Execute();
    bool LSBuffer();
    void Commit();
    void Update();
    void ResetRes() { 
      reg_out[0] = 0; 
    }

   public:
    void Run();
};
#endif