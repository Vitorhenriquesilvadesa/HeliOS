#ifndef HL_INSTRUCTION_H
#define HL_INSTRUCTION_H

#define STACK_MAX_SIZE 32

typedef enum
{
    HL_OP_HALT,
    HL_OP_ADD,
    HL_OP_SUB,
    HL_OP_MUL,
    HL_OP_DIV,
    HL_OP_MOV,
    HL_OP_MOV_CONST,
    HL_OP_MOV_ACC,
    HL_OP_PUSH,
    HL_OP_POP,
    HL_OP_GOTO,
    HL_OP_JEQ,
    HL_OP_JNQ,
    HL_OP_PRINT,
    HL_OP_CREATE_PROCESS,
} Instruction;

typedef enum
{
    REG_R0,
    REG_R1,
    REG_R2,
    REG_R3,
    REG_R4,
    REG_R5,
    REG_R6,
    REG_R7,
    REG_S1,
    REG_MAX,
} Register;

#endif