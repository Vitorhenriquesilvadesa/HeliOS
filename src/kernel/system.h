#ifndef HL_SYSTEM_H
#define HL_SYSTEM_H

#include <kernel.h>

typedef ProcessManager *(ProcManagerCreationFn)(ProcessManagerCreateInfo createInfo);

typedef struct
{
    ProcessManagerCreateInfo procManager;
    ProgramInstantiationFn *programs;
} SystemCreateInfo;

typedef struct HeliOSSystem
{
    ProcessManager *processManager;
    ProgramInstantiationFn *programs;
    Cpu *cpu;
    void *processTable;
} HeliOSSystem;

void createSystemInstance(SystemCreateInfo createInfo);
PID32 createProcess(const char *name, ProgramInstantiationFn program);
PID32 createProcessWithPriority(const char *name, ProgramInstantiationFn program, ProcessPriority priority);
ProcessManager *createProcessManager(ProcessManagerCreateInfo createInfo);
Program getProcessProgram(PID32 pid);

ProgramInstantiationFn getProgramByIndex(Byte index);

void runSystem();
void freeSystemInstance();

void runSystemWithLog(const char *logFile);

#endif