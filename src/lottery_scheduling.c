#include <lottery_scheduling.h>
#include <kernel.h>
#include <time.h>

ProcessManager *createLotterySchedulingProcessManager(ProcessManagerCreateInfo createInfo)
{
    LotterySchedulingProcessManager *manager = ALLOCATE(LotterySchedulingProcessManager, 1);
    manager->manager.type = HL_PROC_MANAGER_TYPE_LOTTERY_SCHEDULING;
    manager->manager.createProc = LSCreateProcess;
    manager->manager.initTable = LSInitProcTable;
    manager->manager.schedule = LSScheduleProcess;
    manager->manager.getProcess = LSGetProcess;
    manager->manager.hasProcess = LSHasProcess;
    manager->manager.onDetach = LSOnProcessDetach;
    manager->procTable = NULL;
    manager->manager.quantum = LOTTERY_QUANTUM;
    manager->manager.preemptionType = PREEMPTION_QUANTUM;

    srand(time(NULL));

    return (ProcessManager *)manager;
}

void *LSInitProcTable(void *self)
{
    LotterySchedulingProcessManager *manager = (LotterySchedulingProcessManager *)self;
    LotteryProcTable *table = ALLOCATE(LotteryProcTable, 1);

    table->head = NULL;
    table->size = 0;
    table->totalTickets = 0;
    initProcessArray(&manager->manager.processes);

    manager->procTable = table;
    return table;
}

Process *LSGetProcess(void *self, PID32 pid)
{
    LotterySchedulingProcessManager *manager = (LotterySchedulingProcessManager *)self;
    return getFromProcessArray(&manager->manager.processes, pid);
}

bool LSHasProcess(void *self)
{
    LotterySchedulingProcessManager *manager = (LotterySchedulingProcessManager *)self;
    return manager->procTable->size > 0;
}

void LSOnProcessDetach(void *self, CpuInfo info)
{
    LotterySchedulingProcessManager *manager = (LotterySchedulingProcessManager *)self;

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
        LotteryProcTable *table = manager->procTable;
        LotteryNode *prev = NULL;
        LotteryNode *current = table->head;

        while (current != NULL)
        {
            if (current->pid == process->pid)
            {
                if (prev == NULL)
                {
                    table->head = current->next;
                }
                else
                {
                    prev->next = current->next;
                }

                table->size--;
                table->totalTickets -= current->tickets;
                free(current);
                break;
            }

            prev = current;
            current = current->next;
        }
    }
}

PID32 LSCreateProcess(void *self, ProcessCreateInfo info)
{
    LotterySchedulingProcessManager *manager = (LotterySchedulingProcessManager *)self;

    static PID32 nextPid = 1;
    PID32 pid = nextPid++;

    Process *process = newProcess(info);

    if (!process)
    {
        fprintf(stderr, "Erro: Falha ao criar o processo.\n");
        return ERROR_PID;
    }

    process->pid = nextPid++;
    if (!process)
    {
        fprintf(stderr, "Erro: Falha ao criar o processo.\n");
        return ERROR_PID;
    }

    process->pid = nextPid++;
    appendProcessReferenceArray(&manager->manager.processes, process);

    static int ticketsPerPriority[HL_PROC_PRIORITY_VERY_HIGH + 1];
    static bool generatedTickets = false;

    if (!generatedTickets)
    {
        generatedTickets = true;
        ticketsPerPriority[HL_PROC_PRIORITY_DEFAULT] = 0;
        ticketsPerPriority[HL_PROC_PRIORITY_VERY_LOW] = 1;
        ticketsPerPriority[HL_PROC_PRIORITY_LOW] = 2;
        ticketsPerPriority[HL_PROC_PRIORITY_MEDIUM] = 5;
        ticketsPerPriority[HL_PROC_PRIORITY_HIGH] = 10;
        ticketsPerPriority[HL_PROC_PRIORITY_VERY_HIGH] = 20;
    }

    int tickets = ticketsPerPriority[info.priority];
    LSEnqueueProcess(self, manager->procTable, process->pid, tickets);

    return pid;
}

PID32 LSScheduleProcess(void *self, void *procTable)
{
    LotteryProcTable *table = (LotteryProcTable *)procTable;

    if (table->size == 0 || table->totalTickets == 0)
    {
        return ERROR_PID;
    }

    int random = rand();
    int winningTicket = random % table->totalTickets;
    int currentSum = 0;

    LotteryNode *current = table->head;
    while (current)
    {
        currentSum += current->tickets;
        if (currentSum > winningTicket)
        {
            return current->pid;
        }
        current = current->next;
    }

    return ERROR_PID;
}

void LSEnqueueProcess(void *self, LotteryProcTable *table, PID32 pid, int tickets)
{
    LotteryNode *node = ALLOCATE(LotteryNode, 1);
    node->pid = pid;
    node->tickets = tickets;
    node->next = table->head;
    table->head = node;

    table->size++;
    table->totalTickets += tickets;
}

PID32 LSDequeueProcess(void *self, LotteryProcTable *table)
{
    if (table->head == NULL)
    {
        return ERROR_PID;
    }

    LotteryNode *nodeToRemove = table->head;
    PID32 pid = nodeToRemove->pid;

    table->head = nodeToRemove->next;
    table->size--;
    table->totalTickets -= nodeToRemove->tickets;

    free(nodeToRemove);

    return pid;
}