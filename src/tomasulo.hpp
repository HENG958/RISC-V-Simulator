#ifndef TOMASULO_H
#define TOMASULO_H
#include <vector>
#include "alu.hpp"
#include "array.hpp"
#include "memory.hpp"
#include "operation.hpp"
#include "queue.hpp"
const int ALU = 5; // ALU的数量
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
        int clk;// Load与Store指令的时钟周期
        void Clear() {
            vj = vk = qj = qk = -1;
            dest = imm = shamt = -1;
            clk = 1;
            current_pc = 0;
        }
    };
    struct RegStatus {
        int reorder;
        bool busy = false;
    };
    struct Status {
    int now_instr_num = 0;          // 当前周期正在执行的指令数量
    int now_instr[ALU];              
    int nxt_instr_num = 0;      // 下一个周期将要执行的指令数量
    int nxt_instr[ALU];          
    int ls;                   
    bool if_update = false;   // 标志是否需要更新加载/存储指令
    };
    Memory memory;
    Alu alu;
    Queue<Rob> rob;
    Queue<RSL> ls;
    Array<RSL> rs;
    RegStatus reg_stat[32];
    Status now;
    int reg_in[32], reg_out[32];
    bool Issue();
    void Execute();
    void Reservation();
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