#include "tomasulo.hpp"
#include "parser.hpp"
#include <iostream>
bool Tomasulo::Issue() {
    //解析指令
    unsigned code = memory.GetInstruction();
    auto instruction = Parse(code);
    //检查ROB是否满
    if (rob.Full()) {
        memory.pc -= 4;
        return false;
    }
    //检查目标寄存器是否处于Busy状态
    if (instruction.rd != -1 && reg_stat[instruction.rd].busy) {
        memory.pc -= 4;
        return false;
    }

    //分配ROB
    int b = rob.Allocate();
    rob[b].Clear();

    //初始化ROB
    rob[b].op = instruction.op;
    rob[b].dest = instruction.rd;
    rob[b].ready = false;
    rob[b].code = code;

    //特殊处理load和store指令
    if (instruction.op >= 11 && instruction.op <= 18) {
        if (ls.Full()) return false; // 检查特殊缓冲区是否满
        //分配特殊缓冲区及初始化
        int s = ls.Allocate();
        ls[s].Clear();
        ls[s].op = instruction.op;
        ls[s].dest = b;
        ls[s].imm = instruction.imm;

        //设置第一个操作数rs1
        if (instruction.rs1 != -1) {
            if (reg_stat[instruction.rs1].busy) {
                int h = reg_stat[instruction.rs1].reorder;
                if (rob[h].ready) {
                    ls[s].vj = rob[h].value;
                    ls[s].qj = -1;
                } else
                    ls[s].qj = h;
            } else {
                ls[s].vj = reg_in[instruction.rs1];
                ls[s].qj = -1;
            }
        }

        //设置第二个操作数rs2
        if (instruction.rs2 != -1) {
            if (reg_stat[instruction.rs2].busy) {
                int h = reg_stat[instruction.rs2].reorder;
                if (rob[h].ready) {
                    ls[s].vk = rob[h].value;
                    ls[s].qk = -1;
                } else
                    ls[s].qk = h;
            } else {
                ls[s].vk = reg_in[instruction.rs2];
                ls[s].qk = -1;
            }
        }

    } else {
        //处理其他指令
        if (rs.Full()) return false;
        rob[b].addr = memory.pc;
        rob[b].offset = instruction.imm;
        int r = rs.Allocate();
        rs[r].Clear();
        rs[r].op = instruction.op;
        rs[r].dest = b;
        rs[r].imm = instruction.imm;
        rs[r].shamt = instruction.shamt;
        rs[r].current_pc = memory.pc;

        if (instruction.rs1 != -1) {
            if (reg_stat[instruction.rs1].busy) {
                int h = reg_stat[instruction.rs1].reorder;
                if (rob[h].ready) {
                    rs[r].vj = rob[h].value;
                    rs[r].qj = -1;
                } else
                    rs[r].qj = h;
            } else {
                rs[r].vj = reg_in[instruction.rs1];
                rs[r].qj = -1;
            }
        }

        if (instruction.rs2 != -1) {
            if (reg_stat[instruction.rs2].busy) {
                int h = reg_stat[instruction.rs2].reorder;
                if (rob[h].ready) {
                    rs[r].vk = rob[h].value;
                    rs[r].qk = -1;
                } else
                    rs[r].qk = h;
            } else {
                rs[r].vk = reg_in[instruction.rs2];
                rs[r].qk = -1;
            }
        }
    }

    //更新寄存器状态
    if (instruction.rd != -1) {
        reg_stat[instruction.rd].reorder = b;
        reg_stat[instruction.rd].busy = true;
    }
    return true;
}

void Tomasulo::Execute() {
    //查找可执行的指令
    int r;
    for (r = 0; r < rs.Length(); r++)
        if (rs.busy[r] && rs[r].qj == -1 && rs[r].qk == -1) break;
    if (r == rs.Length()) return;

    //获取指令相关信息
    RSL &it = rs[r];
    int b = it.dest;
    int ret;

    //执行
    switch (it.op) {
        case Operation::JAL:
            ret = it.current_pc;
            break;
        case Operation::JALR:
            ret = it.current_pc;
            rob[b].pc = (it.vj + it.imm) & ~1;
            break;
        case Operation::LUI:
            ret = it.imm;
            break;
        case Operation::AUIPC:
            ret = it.imm + it.current_pc - 4;
        default:
            ret = alu.Work(it.op, it.vj, it.vk, it.imm, it.shamt);
    }

    //移除
    rs.Erase(r);

    // 更新保留站与特殊缓冲区
    for (int i = 0; i < rs.Length(); i++) {
        if (rs[i].qj == b) {
            rs[i].vj = ret;
            rs[i].qj = -1;
        }
        if (rs[i].qk == b) {
            rs[i].vk = ret;
            rs[i].qk = -1;
        }
    }
    for (int i = 0; i < ls.Length(); i++) {
        if (ls[i].qj == b) {
            ls[i].vj = ret;
            ls[i].qj = -1;
        }
        if (ls[i].qk == b) {
            ls[i].vk = ret;
            ls[i].qk = -1;
        }
    }

    //更新ROB
    rob[b].value = ret;
    rob[b].ready = true;
}

bool Tomasulo::LSBuffer() {
    // 检查特殊缓冲区是否为空
    if (ls.Empty()) return false;

    // 获取特殊缓冲区的第一个指令
    RSL &it = ls.GetFront();

    // 检查指令操作数是否就绪
    if (it.qj != -1 || it.qk != -1) return false;

    //处理load
    if (it.op >= 11 && it.op <= 15) {
        // load
        int b = it.dest;
        rob[b].addr = it.vj + it.imm;
        rob[b].value = memory.Load(it.op, rob[b].addr);
        rob[b].ready = true;
        ls.Pop();
        return true;
    } 
    else {
        // 处理store
        if (rob.GetFrontInd() == it.dest) {
            int b = it.dest;
            rob[b].addr = it.vj + it.imm;
            rob[b].value = it.vk;
            memory.Store(it.op, rob[b].addr, rob[b].value);
            rob[b].ready = true;
            ls.Pop();
            return true;
        } else
            return false;
    }
}

void Tomasulo::Commit() {
    //检查ROB是否为空
    if (rob.Empty()) return;

    //获取ROB的第一个指令及索引
    Rob &it = rob.GetFront();
    int h = rob.GetFrontInd();

    if (it.code == 0x0ff00513) {
        std::cout << static_cast<unsigned>(reg_out[10] & 255u) << std::endl;
        exit(0);
    }
    if (it.ready) {
        int d = it.dest;
        if (it.op >= 5 && it.op <= 10) {
            // 处理branch指令
            //如果分支条件成立，清空ROB、保留站、重置寄存器状态，并跳转
            if (it.value) {
                rob.Clear();
                rs.Clear();
                memset(reg_stat, 0, sizeof(reg_stat));
                memory.pc = it.addr + it.offset - 4;
            }
        } else if (it.op >= 3 && it.op <= 4) {
            // 处理jump指令
            //更新目标寄存器的值，清空ROB、保留站、重置寄存器状态，并跳转
            reg_out[it.dest] = it.value;
            rob.Clear();
            rs.Clear();
            memset(reg_stat, 0, sizeof(reg_stat));
            if (it.op == 3)
                memory.pc = it.addr + it.offset - 4;
            else
                memory.pc = it.pc;
        } else {
            reg_out[it.dest] = it.value;
        }

        //移除
        rob.Pop();
        if (reg_stat[it.dest].reorder == h) reg_stat[it.dest].busy = false;
    }
    return;
}

void Tomasulo::Update() {
    for (int i = 0; i < 32; i++) reg_in[i] = reg_out[i];
}

void Tomasulo::Run() {
    int cnt = 0;
    memory.Read();
    while (true) {
        cnt++;
        Issue();
        ResetRes();

        Execute();
        ResetRes();

        LSBuffer();
        ResetRes();

        Commit();
        ResetRes();

        Update();
        ResetRes();
    }
}