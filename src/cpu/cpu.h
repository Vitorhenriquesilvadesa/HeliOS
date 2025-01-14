#ifndef CPU_H
#define CPU_H

#include <instruction.h>
#include <program.h>
#include <system_types.h>

typedef struct Process Process;
typedef enum ProcessPriority ProcessPriority;

typedef enum
{
    CPU_RUNNING,
    CPU_IDLE,
    CPU_HALTED,
} CpuState;

typedef struct
{
    Byte accumulator;
    Byte pc;
    Byte sp;
    Program currentProgram;
    CpuState state;
    PID32 currentProcessId;
    Byte registers[REG_MAX];
    Byte stack[STACK_MAX_SIZE];
    uint32_t cycles;
} Cpu;

typedef struct CpuInfo
{
    Byte accumulator;
    Byte pc;
    Byte sp;
    Program currentProgram;
    CpuState state;
    PID32 currentProcessId;
    Byte registers[REG_MAX];
    Byte stack[STACK_MAX_SIZE];
} CpuInfo;

void initCpu(Cpu *cpu);
void attachProcess(Cpu *cpu, Process *process);
CpuInfo detachProcess(Cpu *cpu);
void decodeAndExecute(Cpu *cpu);
Byte readByte(Cpu *cpu);
void cpuPush(Cpu *cpu, int constant);
void cpuPop(Cpu *cpu, Register reg);
void cpuAdd(Cpu *cpu, Register reg1, Register reg2);
void cpuSub(Cpu *cpu, Register reg1, Register reg2);
void cpuMul(Cpu *cpu, Register reg1, Register reg2);
void cpuDiv(Cpu *cpu, Register reg1, Register reg2);
void cpuMovConst(Cpu *cpu, Byte value, Register reg1);
void cpuMovAcc(Cpu *cpu, Register reg1);
void cpuJump(Cpu *cpu);
void cpuJumpIfEqual(Cpu *cpu, Register reg, Byte value, Byte index);
void cpuJumpIfNotEqual(Cpu *cpu, Register reg, Byte value, Byte index);
void cpuPrint(Cpu *cpu);
void cpuHalt(Cpu *cpu);
void cpuCreateProcess(Cpu *cpu, ProcessPriority priority, Byte nameIndex, Byte programIndex);

#endif