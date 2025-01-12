#include <cpu.h>
#include <kernel.h>
#include <stdio.h>
#include <process.h>

void initCpu(Cpu *cpu)
{
    cpu->accumulator = 0;
    cpu->currentProcessId = 0;
    cpu->pc = 0;
    cpu->sp = 0;
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
    Byte instruction = readByte(cpu);

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

    case HL_OP_MOV:
        cpuMovConst(cpu, readByte(cpu), readByte(cpu));
        break;

    case HL_OP_PRINT:
        cpuPrint(cpu);
        break;
    }
}

Byte readByte(Cpu *cpu)
{
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
    cpu->accumulator = cpu->registers[reg1] + cpu->registers[reg2];
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

void cpuMovConst(Cpu *cpu, int value, Register reg1)
{
    cpu->registers[reg1] = value;
}

void cpuJump(Cpu *cpu)
{
    cpu->pc = readByte(cpu);
}

void cpuPrint(Cpu *cpu)
{
    printf("%s", &cpu->stack[readByte(cpu)]);
}

void cpuHalt(Cpu *cpu)
{
    cpu->state = CPU_HALTED;
}
