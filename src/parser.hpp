#include "operation.hpp"
int GetImm(unsigned bin_code,Operation::ImmType imm_type);
int GetRs1(unsigned bin_code);
int GetRs2(unsigned bin_code);
int GetRd(unsigned bin_code);
int GetFunct3(unsigned bin_code);
int GetFunct7(unsigned bin_code);
int GetShamt(unsigned bin_code);
Operation Parse(unsigned bin_code);