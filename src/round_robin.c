#include <round_robin.h>
#include <kernel.h>

ProcessManager *createRoundRobinProcessManager(ProcessManagerCreateInfo createInfo)
{
    RoundRobinProcessManager *rr = ALLOCATE(RoundRobinProcessManager, 1);
    rr->manager.type = HL_PROC_MANAGER_TYPE_ROUND_ROBIN;
    rr->manager.createProc = RRCreateProcess;
    rr->manager.initTable = RRInitProcTable;
    rr->manager.schedule = RRScheduleProcess;
    rr->manager.getProcess = RRGetProcess;
    rr->manager.hasProcess = RRHasProcess;
    rr->manager.onDetach = RROnProcessDetach;
    rr->procTable = NULL;
    rr->manager.quantum = ROUND_ROBIN_QUANTUM;
    rr->manager.preemptionType = PREEMPTION_QUANTUM;

    return (ProcessManager *)rr;
}

PID32 RRCreateProcess(void *self, const char *name, Program *program)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;
    RRProcTable *table = manager->procTable;

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

    RRNode *node = ALLOCATE(RRNode, 1);
    node->pid = pid;
    node->next = NULL;

    if (table->size == 0)
    {
        table->current = node;
        table->head = node;
        table->tail = node;
        node->next = node;
    }
    else
    {
        node->next = table->head;
        table->tail->next = node;
        table->tail = node;
    }

    table->size++;

    return pid;
}

PID32 RRScheduleProcess(void *self, void *procTable)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;

    RRProcTable *table = (RRProcTable *)procTable;

    PID32 pid = table->current->pid;
    table->current = table->current->next;

    return pid;
}

void *RRInitProcTable(void *self)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;
    RRProcTable *table = ALLOCATE(RRProcTable, 1);

    table->head = NULL;
    table->tail = NULL;
    table->current = NULL;
    table->size = 0;
    initProcessArray(&manager->manager.processes);

    manager->procTable = table;
    return (void *)table;
}

Process *RRGetProcess(void *self, PID32 pid)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;
    RRProcTable *table = manager->procTable;

    if (table->size == 0)
        return NULL;

    RRNode *current = table->head;

    for (size_t i = 0; i < table->size; i++)
    {
        if (current->pid == pid)
            return getFromProcessArray(&manager->manager.processes, pid);
        current = current->next;
    }

    return NULL;
}

bool RRHasProcess(void *self)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;
    RRProcTable *table = manager->procTable;
    return table->size > 0;
}

void RROnProcessDetach(void *self, CpuInfo info)
{
    RoundRobinProcessManager *manager = (RoundRobinProcessManager *)self;

    Process *process = getFromProcessArray(&manager->manager.processes, info.currentProcessId);

    process->accumulator = info.accumulator;
    process->pc = info.pc;
    process->sp = info.sp;

    for (int i = 0; i < REG_MAX; i++)
    {
        process->registers[i] = info.registers[i];
    }

    for (int i = 0; i < STACK_MAX_SIZE; i++)
    {
        process->stack[i] = info.stack[i];
    }

    if (process->pc >= process->program->count)
    {
        RRNode *current = manager->procTable->head;
        RRNode *prev = NULL;

        RRProcTable *rrtable = manager->procTable;

        do
        {
            if (current->pid == info.currentProcessId)
            {
                if (current == rrtable->head && rrtable->size == 1)
                {
                    rrtable->head = NULL;
                    rrtable->current = NULL;
                }
                else
                {
                    if (current == rrtable->head)
                    {
                        rrtable->head = current->next;
                    }

                    if (prev)
                    {
                        prev->next = current->next;
                    }

                    if (rrtable->current == current)
                    {
                        rrtable->current = current->next;
                    }
                }

                free(current);
                rrtable->size--;
                break;
            }

            prev = current;
            current = current->next;
        } while (current != rrtable->head);
    }
}
