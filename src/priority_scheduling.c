#include <priority_scheduling.h>
#include <kernel.h>

ProcessManager *createPrioritySchedulingSingleQueueProcessManager(ProcessManagerCreateInfo createInfo)
{
    PrioritySchedulingSingleQueueProcessManager *manager = ALLOCATE(PrioritySchedulingSingleQueueProcessManager, 1);
    manager->manager.type = HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_SINGLE_QUEUE;
    manager->manager.createProc = PSSQCreateProcess;
    manager->manager.initTable = PSSQInitProcTable;
    manager->manager.schedule = PSSQScheduleProcess;
    manager->manager.getProcess = PSSQGetProcess;
    manager->manager.hasProcess = PSSQHasProcess;
    manager->manager.onDetach = PSSQOnProcessDetach;
    manager->procTable = NULL;
    manager->manager.quantum = QUANTUM_NONE;
    manager->manager.preemptionType = PREEMPTION_PRIORITY;

    return (ProcessManager *)manager;
}

PID32 PSSQCreateProcess(void *self, const char *name, Program *program, ProcessPriority priority)
{
    static PID32 nextPid = 1;
    PID32 pid = nextPid++;

    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;

    ProcessCreateInfo createInfo = {
        .arrivalTime = 0,
        .burstTime = program->count,
        .name = name,
        .priority = priority,
    };

    Process *process = newProcess(createInfo, program);

    if (!process)
    {
        fprintf(stderr, "Erro: Falha ao criar o processo.\n");
        return ERROR_PID;
    }

    process->pid = pid;
    appendProcessReferenceArray(&manager->manager.processes, process);

    PSSQEnqueueProcess(self, manager->procTable, pid);

    return pid;
}

PID32 PSSQScheduleProcess(void *self, void *procTable)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    PriorityProcTable *table = (PriorityProcTable *)procTable;

    const PID32 pid = PSSQDequeueProcess(self, table);

    if (pid != ERROR_PID)
    {
        Process *process = getFromProcessArray(&manager->manager.processes, pid);
        return pid;
    }

    return ERROR_PID;
}

void *PSSQInitProcTable(void *self)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    PriorityProcTable *table = ALLOCATE(PriorityProcTable, 1);

    table->head = NULL;
    table->tail = NULL;
    table->size = 0;
    initProcessArray(&manager->manager.processes);

    manager->procTable = table;
    return table;
}

Process *PSSQGetProcess(void *self, PID32 pid)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    return getFromProcessArray(&manager->manager.processes, pid);
}

bool PSSQHasProcess(void *self)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    return manager->procTable->size > 0;
}

void PSSQOnProcessDetach(void *self, CpuInfo info)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    removeFromProcessArray(&manager->manager.processes, info.currentProcessId);
}

void PSSQEnqueueProcess(void *self, PriorityProcTable *table, PID32 pid)
{
    PrioritySchedulingSingleQueueProcessManager *manager = (PrioritySchedulingSingleQueueProcessManager *)self;
    Process *process = getFromProcessArray(&manager->manager.processes, pid);

    PriorityNode *node = ALLOCATE(PriorityNode, 1);
    node->pid = pid;
    node->next = NULL;

    if (table->head == NULL)
    {
        table->head = node;
        table->tail = node;
    }
    else
    {
        PriorityNode *current = table->head;
        PriorityNode *previous = NULL;

        while (current != NULL &&
               process->priority < getFromProcessArray(&manager->manager.processes, current->pid)->priority)
        {
            previous = current;
            current = current->next;
        }

        if (previous == NULL)
        {
            node->next = table->head;
            table->head = node;
        }
        else
        {
            previous->next = node;
            node->next = current;

            if (current == NULL)
            {
                table->tail = node;
            }
        }
    }

    table->size++;
}

PID32 PSSQDequeueProcess(void *self, PriorityProcTable *table)
{
    if (table->head == NULL)
    {
        fprintf(stderr, "Erro: Nenhum processo na fila de prioridade.\n");
        return ERROR_PID;
    }

    PriorityNode *nodeToRemove = table->head;
    PID32 pid = nodeToRemove->pid;

    table->head = nodeToRemove->next;
    if (table->head == NULL)
    {
        table->tail = NULL;
    }

    free(nodeToRemove);
    table->size--;

    return pid;
}