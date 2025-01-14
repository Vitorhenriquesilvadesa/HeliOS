#include <system.h>
#include <first_come_first_served.h>
#include <round_robin.h>
#include <shortest_job_first.h>
#include <priority_scheduling_single_queue.h>
#include <priority_scheduling_multiple_queues.h>
#include <lottery_scheduling.h>

static HeliOSSystem systemInstance;

void createSystemInstance(SystemCreateInfo createInfo)
{
    ProcessManager *manager = createProcessManager(createInfo.procManager);
    initCpu(&systemInstance.cpu);
    systemInstance.processTable = manager->initTable(manager);
    systemInstance.processManager = manager;
    systemInstance.programs = createInfo.programs;
}

PID32 createProcess(const char *name, ProgramInstantiationFn program)
{
    Program *programPtr = program();

    ProcessCreateInfo createInfo = {
        .name = name,
        .program = programPtr,
        .priority = HL_PROC_PRIORITY_DEFAULT,
        .burstTime = programPtr->count,
        .arrivalTime = systemInstance.cpu.cycles,
    };

    ProcessManager *manager = systemInstance.processManager;
    return manager->createProc(manager, createInfo);
}

PID32 createProcessWithPriority(const char *name, ProgramInstantiationFn program, ProcessPriority priority)
{
    Program *programPtr = program();

    ProcessCreateInfo createInfo = {
        .name = name,
        .program = programPtr,
        .priority = priority,
        .burstTime = programPtr->count,
        .arrivalTime = systemInstance.cpu.cycles,
    };

    ProcessManager *manager = systemInstance.processManager;
    return manager->createProc(manager, createInfo);
}

ProcessManager *createProcessManager(ProcessManagerCreateInfo createInfo)
{
    static ProcManagerCreationFn *procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_MAX];
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED] = createFirstComeFirstServedProcessManager;
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_ROUND_ROBIN] = createRoundRobinProcessManager;
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_SHORTEST_JOB_FIRST] = createShortestJobFirstProcessManager;
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_SINGLE_QUEUE] = createPrioritySchedulingSingleQueueProcessManager;
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES] = createPrioritySchedulingMultipleQueueProcessManager;
    procManagerCreationFunctions[HL_PROC_MANAGER_TYPE_LOTTERY_SCHEDULING] = createLotterySchedulingProcessManager;

    return procManagerCreationFunctions[createInfo.type](createInfo);
}

Program getProcessProgram(PID32 pid)
{
    ProcessManager *manager = systemInstance.processManager;
    return *manager->getProcess(manager, pid)->program;
}

ProgramInstantiationFn getProgramByIndex(Byte index)
{
    return systemInstance.programs[index];
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
        printf("Running now: %s, PC: %d, COUNT: %d\n", process->name, process->pc, process->program->count);

        int cycles = 0;

        if (manager->preemptionType & PREEMPTION_QUANTUM)
        {
            while (cpu.state != CPU_HALTED && cycles < manager->quantum)
            {
                if (cpu.pc == cpu.currentProgram.count)
                {
                    break;
                }
                decodeAndExecute(&cpu);
                cycles++;
                cpu.cycles++;
            }
        }
        else
        {
            while (cpu.state != CPU_HALTED)
            {
                if (cpu.pc == cpu.currentProgram.count)
                {
                    break;
                }
                decodeAndExecute(&cpu);
            }
        }

        if (cpu.pc == process->program->count)
        {
            process->state = HL_PROC_TERMINATED;
        }

        CpuInfo info = detachProcess(&cpu);
        // printf("Detaching now: %s, PC: %d, COUNT: %d\n", process->name, info.pc, info.currentProgram.count);

        manager->onDetach(manager, info);
    }
}
