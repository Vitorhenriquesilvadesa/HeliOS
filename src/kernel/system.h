#ifndef HL_SYSTEM_H
#define HL_SYSTEM_H

#include <kernel.h>

typedef struct
{
    ProcessManagerCreateInfo procManager;
} SystemCreateInfo;

typedef struct
{
    ProcessManager *processManager;
    Cpu cpu;
    void *processTable;
} HeliOSSystem;

void createSystemInstance(SystemCreateInfo createInfo);
PID32 createProcess(const char *name, ProgramInstantiationFn program);
PID32 createProcessWithPriority(const char *name, ProgramInstantiationFn program, ProcessPriority priority);
ProcessManager *createProcessManager(ProcessManagerCreateInfo createInfo);
Program getProcessProgram(PID32 pid);

void runSystem();

#endif