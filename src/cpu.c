#include <cpu.h>
#include <kernel.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

typedef struct HeliOSSystem HeliOSSystem;

extern PID32 createProcessWithPriority(const char *name, ProgramInstantiationFn program, ProcessPriority priority);
extern ProgramInstantiationFn getProgramByIndex(Byte index);

void initCpu(Cpu *cpu)
{
    cpu->accumulator = 0;
    cpu->currentProcessId = 0;
    cpu->pc = 0;
    cpu->sp = 0;
    cpu->cycles = 0;
    cpu->currentProcessId = ERROR_PID;
    cpu->state = CPU_IDLE;
}

void attachProcess(Cpu *cpu, Process *process)
{
    cpu->accumulator = process->accumulator;
    cpu->currentProcessId = process->pid;
    cpu->pc = process->pc;
    cpu->sp = process->sp;
    cpu->state = CPU_RUNNING;
    cpu->currentProgram = *process->program;

    for (int i = 0; i < REG_MAX; i++)
    {
        cpu->registers[i] = process->registers[i];
    }

    for (int i = 0; i < STACK_MAX_SIZE; i++)
    {
        cpu->stack[i] = process->stack[i];
    }
}

CpuInfo detachProcess(Cpu *cpu)
{
    CpuInfo info;

    info.accumulator = cpu->accumulator;
    info.currentProcessId = cpu->currentProcessId;
    info.currentProgram = cpu->currentProgram;
    info.pc = cpu->pc;
    info.sp = cpu->sp;
    info.state = CPU_RUNNING;

    for (int i = 0; i < REG_MAX; i++)
    {
        info.registers[i] = cpu->registers[i];
    }

    for (int i = 0; i < STACK_MAX_SIZE; i++)
    {
        info.stack[i] = cpu->stack[i];
    }

    return info;
}

void decodeAndExecute(Cpu *cpu)
{
    Instruction instruction = (Instruction)readByte(cpu);

    switch (instruction)
    {
    case HL_OP_HALT:
        cpuHalt(cpu);
        return;

    case HL_OP_ADD:
        cpuAdd(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_SUB:
        cpuSub(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_MUL:
        cpuMul(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_DIV:
        cpuDiv(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_GOTO:
        cpuJump(cpu);
        break;

    case HL_OP_PUSH:
        cpuPush(cpu, readByte(cpu));
        break;

    case HL_OP_MOV_CONST:
        cpuMovConst(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_MOV_ACC:
        cpuMovAcc(cpu, readByte(cpu));
        break;

    case HL_OP_PRINT:
        cpuPrint(cpu);
        break;

    case HL_OP_JEQ:
        cpuJumpIfEqual(cpu, readByte(cpu), readByte(cpu), readByte(cpu));
        break;

    case HL_OP_JNQ:
        cpuJumpIfNotEqual(cpu, readByte(cpu), readByte(cpu), readByte(cpu));
        break;
        break;

    case HL_OP_CREATE_PROCESS:
        cpuCreateProcess(cpu, readByte(cpu), readByte(cpu), readByte(cpu));
        break;
    }
}

Byte readByte(Cpu *cpu)
{
    cpu->cycles++;

    return cpu->currentProgram.instructions[cpu->pc++];
}

void cpuPush(Cpu *cpu, int constant)
{
    if (cpu->sp == STACK_MAX_SIZE - 1)
    {
        fprintf(stderr, "Stack overflow.");
        cpuHalt(cpu);
        return;
    }

    cpu->stack[cpu->sp++] = constant;
}

void cpuPop(Cpu *cpu, Register reg)
{
    if (cpu->sp == -1)
    {
        fprintf(stderr, "Stack underflow.");
        cpuHalt(cpu);
        return;
    }

    cpu->registers[reg] = cpu->stack[cpu->sp--];
}

void cpuAdd(Cpu *cpu, Register reg1, Register reg2)
{
    Byte result = cpu->registers[reg1] + cpu->registers[reg2];
    cpu->accumulator = result;
}

void cpuSub(Cpu *cpu, Register reg1, Register reg2)
{
    cpu->accumulator = cpu->registers[reg1] - cpu->registers[reg2];
}

void cpuMul(Cpu *cpu, Register reg1, Register reg2)
{
    cpu->accumulator = cpu->registers[reg1] * cpu->registers[reg2];
}

void cpuDiv(Cpu *cpu, Register reg1, Register reg2)
{
    cpu->accumulator = cpu->registers[reg1] / cpu->registers[reg2];
}

void cpuMovConst(Cpu *cpu, Byte value, Register reg1)
{
    cpu->registers[reg1] = value;
}

void cpuMovAcc(Cpu *cpu, Register reg1)
{
    cpu->registers[reg1] = cpu->accumulator;
}

void cpuJump(Cpu *cpu)
{
    cpu->pc = readByte(cpu);
}

void cpuJumpIfEqual(Cpu *cpu, Register reg, Byte value, Byte index)
{
    if (cpu->registers[reg] == value)
    {
        cpu->pc = index;
    }
}

void cpuJumpIfNotEqual(Cpu *cpu, Register reg, Byte value, Byte index)
{
    if (cpu->registers[reg] != value)
    {
        cpu->pc = index;
    }
}

void cpuPrint(Cpu *cpu)
{
    Byte index = readByte(cpu);
    printf("%s", &cpu->stack[index]);
    cpu->sp = index;
}

void cpuHalt(Cpu *cpu)
{
    cpu->state = CPU_HALTED;
}

void cpuCreateProcess(Cpu *cpu, ProcessPriority priority, Byte nameIndex, Byte programIndex)
{
    char *name = ALLOCATE(char, (strlen(&cpu->stack[nameIndex]) + 1));
    strcpy(name, &cpu->stack[nameIndex]);
    createProcessWithPriority((const char *)name, getProgramByIndex(programIndex), priority);
}
