#include <first_come_first_served.h>
#include <kernel.h>
#include <cpu.h>

ProcessManager *createFirstComeFirstServedProcessManager(ProcessManagerCreateInfo createInfo)
{
    FirstComeFirstServedProcessManager *fcfs = ALLOCATE(FirstComeFirstServedProcessManager, 1);
    fcfs->manager.type = HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED;
    fcfs->manager.createProc = FCFSCreateProcess;
    fcfs->manager.initTable = FCFSInitProcTable;
    fcfs->manager.schedule = FCFSScheduleProcess;
    fcfs->manager.getProcess = FCFSGetProcess;
    fcfs->manager.hasProcess = FCFSHasProcess;
    fcfs->manager.onDetach = FCFSOnProcessDetach;
    fcfs->procTable = NULL;
    fcfs->manager.quantum = QUANTUM_NONE;
    fcfs->manager.preemptionType = PREEMPTION_NONE;

    return (ProcessManager *)fcfs;
}

PID32 FCFSCreateProcess(void *self, const char *name, Program *program)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;
    FCFSProcTable *table = manager->procTable;

    static PID32 nextPid = 1;

    PID32 pid = nextPid++;

    ProcessCreateInfo createInfo = {
        .arrivalTime = 0,
        .burstTime = 0,
        .name = name,
        .priority = HL_PROC_PRIORITY_DEFAULT,
    };

    Process *process = newProcess(createInfo, program);
    process->pid = pid;

    appendProcessReferenceArray(&manager->manager.processes, process);
    FCFSEnqueueProcess(self, table, pid);

    return pid;
}

PID32 FCFSScheduleProcess(void *self, void *procTable)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;
    FCFSProcTable *table = (FCFSProcTable *)procTable;

    const PID32 pid = FCFSDequeueProcess(self, table);

    if (pid != ERROR_PID)
    {
        Process *process = getFromProcessArray(&manager->manager.processes, pid);
        return pid;
    }

    return ERROR_PID;
}

void *FCFSInitProcTable(void *self)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;
    FCFSProcTable *table = ALLOCATE(FCFSProcTable, 1);

    table->head = NULL;
    table->tail = NULL;
    table->size = 0;
    initProcessArray(&manager->manager.processes);

    manager->procTable = table;
    return (void *)table;
}

Process *FCFSGetProcess(void *self, PID32 pid)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;

    return getFromProcessArray(&manager->manager.processes, pid);
}

bool FCFSHasProcess(void *self)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;

    return manager->procTable->size > 0;
}

void FCFSOnProcessDetach(void *self, CpuInfo info)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;
}

void FCFSEnqueueProcess(void *self, FCFSProcTable *table, PID32 pid)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;

    FCFSNode *node = ALLOCATE(FCFSNode, 1);

    node->pid = pid;
    node->next = NULL;

    if (table->tail)
    {
        table->tail->next = node;
    }
    else
    {
        table->head = node;
    }

    table->tail = node;
    table->size++;
}

PID32 FCFSDequeueProcess(void *self, FCFSProcTable *table)
{
    FirstComeFirstServedProcessManager *manager = (FirstComeFirstServedProcessManager *)self;

    if (!table->head)
    {
        fprintf(stderr, "Erro: Nenhum processo na fila FCFS.\n");
        return ERROR_PID;
    }

    FCFSNode *nodeToRemove = table->head;
    PID32 pid = nodeToRemove->pid;

    table->head = nodeToRemove->next;
    if (!table->head)
    {
        table->tail = NULL;
    }

    free(nodeToRemove);

    table->size--;
    return pid;
}
