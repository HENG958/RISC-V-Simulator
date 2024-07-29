#ifndef OPERATION
#define OPERATION
struct Operation {
    enum ImmType { R, I, S, B, U, J };
    enum Oper {
        LUI,
        AUIPC,
        JAL,
        JALR,
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,
        LB,
        LH,
        LW,
        LBU,
        LHU,
        SB,
        SH,
        SW,
        ADDI,
        SLTI,
        SLTIU,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND
    };
    Oper op;
    int rs1 = 0, rs2 = 0, rd = 0;
    int shamt = 0;
    int imm = 0;
};
#endif