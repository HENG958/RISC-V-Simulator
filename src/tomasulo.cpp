#include "tomasulo.hpp"

#include <iostream>

#include "parser.hpp"
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

    if (instruction.op >= 5 && instruction.op <= 10) {
        if (bit_status >= 2) {
            memory.pc += rob[b].offset - 4;
            rob[b].prediction = 1;
        } else {
            rob[b].prediction = 0;
        }
    }
}

void Tomasulo::Execute() {
    for (int i = 0; i < now.now_instr_num; i++) {
        RSL &it = rs[now.now_instr[i]];
        int b = it.dest;
        switch (it.op) {
            case Operation::JAL:
                rob[b].value = it.current_pc;
                break;
            case Operation::JALR:
                rob[b].value = it.current_pc;
                rob[b].pc = (it.vj + it.imm) & ~1;
                break;
            case Operation::LUI:
                rob[b].value = it.imm;
                break;
            case Operation::AUIPC:
                rob[b].value = it.imm + it.current_pc - 4;
                break;
            default:
                rob[b].value = alu.Work(it.op, it.vj, it.vk, it.imm, it.shamt);
        }
        rs.Erase(now.now_instr[i]);
        rob[b].ready = true;
    }
}

bool Tomasulo::LSBuffer() {
    // 检查特殊缓冲区是否为空
    if (ls.Empty()) return false;

    // 获取特殊缓冲区的第一个指令
    RSL &it = ls.GetFront();

    // 检查指令操作数是否就绪
    if (it.qj != -1 || it.qk != -1) return false;

    if (it.clk != 3) {
        it.clk++;
        return false;
    }
    //处理load
    if (it.op >= 11 && it.op <= 15) {
        // load
        int b = it.dest;
        rob[b].addr = it.vj + it.imm;
        rob[b].value = memory.Load(it.op, rob[b].addr);
        rob[b].ready = true;
        ls.Pop();
        now.ls = b;
        now.if_update = true;
        return true;
    } 
    else {
        // 处理store
        if (rob.GetFrontInd() == it.dest) {
            int b = it.dest;
            rob[b].addr = it.vj + it.imm;
            memory.Store(it.op, rob[b].addr, it.vk);
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
            if (it.value ^ it.prediction) {
                rob.Clear();
                rs.Clear();
                ls.Clear();
                now.Clear();  
                memset(reg_stat, 0, sizeof(reg_stat));
                if (it.value)
                    memory.pc = it.addr + it.offset - 4;
                else
                    memory.pc = it.addr;

                if (it.prediction && bit_status > 0)
                    bit_status--;
                else if (!it.prediction && bit_status < 3)
                    bit_status++;
            }
        } else if (it.op >= 3 && it.op <= 4) {
            // 处理jump指令
            //更新目标寄存器的值，清空ROB、保留站、重置寄存器状态，并跳转
            reg_out[it.dest] = it.value;
            rob.Clear();
            rs.Clear();
            ls.Clear();
            now.Clear();
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

    // 遍历当前执行的指令
    for (int j = 0; j < now.now_instr_num; j++) {
        int b = rs[now.now_instr[j]].dest; // 获取指令的目标寄存器
        // 更新保留站中的值
        for (int i = 0; i < rs.Length(); i++) {
            if (rs[i].qj == b) {
                rs[i].vj = rob[b].value; // 更新操作数 vj
                rs[i].qj = -1; // 标记操作数 vj 就绪
            }
            if (rs[i].qk == b) {
                rs[i].vk = rob[b].value; // 更新操作数 vk
                rs[i].qk = -1; // 标记操作数 vk 就绪
            }
        }
        // 更新加载/存储缓冲区中的值
        for (int i = 0; i <= ls.Length(); i++) {
            if (ls[i].qj == b) {
                ls[i].vj = rob[b].value; // 更新操作数 vj
                ls[i].qj = -1; // 标记操作数 vj 就绪
            }
            if (ls[i].qk == b) {
                ls[i].vk = rob[b].value; // 更新操作数 vk
                ls[i].qk = -1; // 标记操作数 vk 就绪
            }
        }
    }

    // 如果需要更新加载/存储指令
    if (now.if_update) {
        int b = now.ls; // 获取当前加载/存储指令的目标寄存器
        // 更新保留站中的值
        for (int i = 0; i < rs.Length(); i++) {
            if (rs[i].qj == b) {
                rs[i].vj = rob[b].value; // 更新操作数 vj
                rs[i].qj = -1; // 标记操作数 vj 就绪
            }
            if (rs[i].qk == b) {
                rs[i].vk = rob[b].value; // 更新操作数 vk
                rs[i].qk = -1; // 标记操作数 vk 就绪
            }
        }
        // 更新加载/存储缓冲区中的值
        for (int i = 0; i <= ls.Length(); i++) {
            if (ls[i].qj == b) {
                ls[i].vj = rob[b].value; // 更新操作数 vj
                ls[i].qj = -1; // 标记操作数 vj 就绪
            }
            if (ls[i].qk == b) {
                ls[i].vk = rob[b].value; // 更新操作数 vk
                ls[i].qk = -1; // 标记操作数 vk 就绪
            }
        }
    }

    // 重置加载/存储更新标志
    now.if_update = false;

    // 更新当前执行的指令集
    for (int i = 0; i < now.nxt_instr_num; i++) now.now_instr[i] = now.nxt_instr[i];
    now.now_instr_num = now.nxt_instr_num;
}

void Tomasulo::Reservation() {
    now.nxt_instr_num = 0; // 初始化下一个执行周期的指令数量为 0

    // 遍历保留站中的所有条目，直到找到足够多的可执行指令
    for (int r = 0; r < rs.Length() && now.nxt_instr_num < ALU; r++) {
        // 检查保留站条目是否忙碌且操作数 qj 和 qk 都已经就绪
        if (rs.busy[r] && rs[r].qj == -1 && rs[r].qk == -1) {
            // 将该条目添加到下一个执行周期的指令集中
            now.nxt_instr[now.nxt_instr_num++] = r;
            rs[r].state = 1; // 标记该条目为已执行
        }
    }
}

void Tomasulo::Run() {
    int clk = 0;
    memory.Read();
    ls.Clear();
    rs.Clear();
    rob.Clear();
    memset(reg_stat, 0, sizeof(reg_stat));
    while (true) {
        clk++;
        Issue();
        ResetRes();

        Execute();
        ResetRes();

        Reservation();
        ResetRes();

        LSBuffer();
        ResetRes();

        Commit();
        ResetRes();

        Update();
        ResetRes();
    }
}