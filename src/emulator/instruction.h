#ifndef INSTUCTION
#define INSTUCTION

#define N_SHIFT 31
#define Z_SHIFT 30
#define C_SHIFT 29
#define V_SHIFT 28

#define N(cpsr) ((cpsr & N_MASK) >> CPSR_N_SHIFT)
#define Z(cpsr) ((cpsr & CPSR_Z_MASK) >> CPSR_Z_SHIFT)
#define C(cpsr) ((cpsr & CPSR_C_MASK) >> CPSR_C_SHIFT)
#define V(cpsr) ((cpsr & CPSR_V_MASK) >> CPSR_V_SHIFT)

#define GET_CPSR_FLAGS(cpsr) (cpsr >> COND_SHIFT)
#define UPDATE_CPSR(instruction) (instruction & INSTRUCTION_S_MASK)

#define DPI_MASK 0x0C000000
#define IS_DPI 0x00000000
#define MULT_MASK 0x0FC000F0
#define IS_MULT 0x0000090
#define SDT_MASK 0x0C600000
#define IS_SDT 0x04000000
#define BRANCH_MASK 0x0F000000
#define IS_BRANCH 0x0A000000

#define CSPR_SHIFT 20
#define CSPR_MASK 0x00100000
#define COND_SHIFT 28
#define COND_MASK 0xF0000000

#define DPI_I 0x02000000 // 25
#define DPI_OPCODE 0x01E00000 // 21-24
#define DPI_RN 0xF0000 // 16-20
#define DPI_RD 0xF000 // 12-15
#define DPI_OP2 0x00000FFF // 0-11
#define DPI_I_SHIFT 25
#define DPI_OPCODE_SHIFT 21
#define DPI_RN_SHIFT 16
#define DPI_RD_SHIFT 12
#define ROTATION 8
#define ROTATION_MULT 2

#define ACC_MASK 0x00200000
#define RS_MASK 0x00000F00
#define RN_MASK 0x0000F000
#define RD_MASK 0x000F0000
#define RM_MASK 0x0000000F
#define ACC_SHIFT 21
#define RS_SHIFT 8
#define RN_SHIFT 12
#define RD_SHIFT 16

#define SDTI_I_MASK 0x02000000
#define SDTI_P_MASK 0x01000000
#define SDTI_U_MASK 0x00800000
#define SDTI_L_MASK 0x00100000
#define SDTI_RN_MASK 0x000F0000
#define SDTI_RD_MASK 0x0000F000
#define SDTI_OFFSET_MASK 0x00000FFF
#define SDTI_I_SHIFT 25
#define SDTI_P_SHIFT 24
#define SDTI_U_SHIFT 23
#define SDTI_L_SHIFT 20
#define SDTI_RN_SHIFT 16
#define SDTI_RD_SHIFT 12

#define OFFSET 0x00FFFFFF

#endif
