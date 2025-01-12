#include <system.h>
#include <first_come_first_served.h>
#include <round_robin.h>

static HeliOSSystem systemInstance;

typedef ProcessManager *(ProcManagerCreationFn)(ProcessManagerCreateInfo createInfo);

void createSystemInstance(SystemCreateInfo createInfo)
{
    ProcessManager *manager = createProcessManager(createInfo.procManager);
    initCpu(&systemInstance.cpu);
    systemInstance.processTable = manager->initTable(manager);
    systemInstance.processManager = manager;
}

PID32 createProcess(const char *name, Program *program)
{
    ProcessManager *manager = systemInstance.processManager;
    return manager->createProc(manager, name, program);
}

ProcessManager *createProcessManager(ProcessManagerCreateInfo createInfo)
{
    static ProcManagerCreationFn *procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_MAX];
        procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED] = createFirstComeFirstServedProcessManager;
        procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_ROUND_ROBIN] = createRoundRobinProcessManager;


    return procManagerCreationFunctions[createInfo.type](createInfo);
}

Program getProcessProgram(PID32 pid)
{
    ProcessManager *manager = systemInstance.processManager;
    return *manager->getProcess(manager, pid)->program;
}

void runSystem()
{
    ProcessManager *manager = systemInstance.processManager;
    Cpu cpu = systemInstance.cpu;

    while (manager->hasProcess(manager))
    {
        PID32 pid = manager->schedule(manager, systemInstance.processTable);
        Process *process = manager->getProcess(manager, pid);

        attachProcess(&cpu, process);
        printf("Running now: %s\n", process->name);

        int cycles = 0;

        if (manager->preemptionType == PREEMPTION_QUANTUM)
        {
            int cycles = 0;
            while (cpu.state != CPU_HALTED && cycles != manager->quantum)
            {
                decodeAndExecute(&cpu);
                cycles++;
            }
        }
        else
        {
            while (cpu.state != CPU_HALTED)
            {
                decodeAndExecute(&cpu);
            }
        }

        CpuInfo info = detachProcess(&cpu);
        manager->onDetach(manager, info);
    }
}
