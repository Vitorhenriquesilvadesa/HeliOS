#include <priority_scheduling_multiple_queues.h>
#include <kernel.h>

ProcessManager *createPrioritySchedulingMultipleQueueProcessManager(ProcessManagerCreateInfo createInfo)
{
    PrioritySchedulingMultipleQueueProcessManager *psmq = ALLOCATE(PrioritySchedulingMultipleQueueProcessManager, 1);

    psmq->manager.type = HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES;
    psmq->manager.createProc = PSMQCreateProcess;
    psmq->manager.getProcess = PSMQGetProcess;
    psmq->manager.hasProcess = PSMQHasProcess;
    psmq->manager.initTable = PSMQInitProcTable;
    psmq->manager.onDetach = PSMQOnProcessDetach;
    psmq->manager.schedule = PSMQScheduleProcess;
    psmq->manager.preemptionType = PREEMPTION_QUANTUM;
    psmq->manager.quantum = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM;

    return (ProcessManager *)psmq;
}

PID32 PSMQCreateProcess(void *self, ProcessCreateInfo info)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;

    static PID32 nextPid = 0;

    PID32 pid = nextPid++;

    Process *process = newProcess(info);
    process->pid = pid;

    appendProcessReferenceArray(&manager->manager.processes, process);
    PSMQEnqueueProcess(self, manager->table, pid, info.priority);

    return pid;
}

PID32 PSMQScheduleProcess(void *self, void *procTable)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;
    PSMQProcTable *table = (PSMQProcTable *)procTable;

    for (int i = HL_PROC_PRIORITY_VERY_HIGH; i >= 0; i--)
    {
        PSMQQueue *queue = manager->table->queues[i];
        PSMQNode *current = queue->head;

        while (current != NULL)
        {
            if (getFromProcessArray(&manager->manager.processes, current->pid)->state != HL_PROC_TERMINATED)
            {
                table->currentNode = current;
                table->currentPriority = i;
                manager->manager.quantum = table->quantums[i];
                return current->pid;
            }
        }
    }

    return ERROR_PID;
}

void *PSMQInitProcTable(void *self)
{
    PrioritySchedulingMultipleQueueProcessManager *psmq = (PrioritySchedulingMultipleQueueProcessManager *)self;

    initProcessArray(&psmq->manager.processes);

    psmq->table = ALLOCATE(PSMQProcTable, 1);

    psmq->table->quantums[HL_PROC_PRIORITY_DEFAULT] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 1;
    psmq->table->quantums[HL_PROC_PRIORITY_VERY_LOW] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 2;
    psmq->table->quantums[HL_PROC_PRIORITY_LOW] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 3;
    psmq->table->quantums[HL_PROC_PRIORITY_MEDIUM] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 5;
    psmq->table->quantums[HL_PROC_PRIORITY_HIGH] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 7;
    psmq->table->quantums[HL_PROC_PRIORITY_VERY_HIGH] = PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM * 10;

    for (int i = 0; i < HL_PROC_PRIORITY_VERY_HIGH + 1; i++)
    {
        psmq->table->queues[i] = ALLOCATE(PSMQQueue, 1);
        psmq->table->queues[i]->head = NULL;
        psmq->table->queues[i]->tail = NULL;
        psmq->table->queues[i]->length = 0;
        psmq->table->queues[i]->quantum = psmq->table->quantums[i];
    }

    psmq->table->currentNode = NULL;
    psmq->table->currentIndex = 0;
    psmq->table->size = 0;
    psmq->table->currentPriority = HL_PROC_PRIORITY_VERY_HIGH;

    return (void *)psmq->table;
}

Process *PSMQGetProcess(void *self, PID32 pid)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;

    return getFromProcessArray(&manager->manager.processes, pid);
}

bool PSMQHasProcess(void *self)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;

    return manager->table->size > 0;
}

void PSMQOnProcessDetach(void *self, CpuInfo info)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;

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

    if (process->state == HL_PROC_TERMINATED)
    {
        PSMQDequeueProcess(self, manager->table, process->priority, info.currentProcessId);
    }
}

void PSMQEnqueueProcess(void *self, PSMQProcTable *table, PID32 pid, ProcessPriority priority)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;
    PSMQQueue *queue = manager->table->queues[priority];

    PSMQNode *node = ALLOCATE(PSMQNode, 1);
    node->next = NULL;
    node->pid = pid;
    node->priority = priority;

    if (queue->length == 0)
    {
        queue->head = node;
        queue->tail = node;
    }
    else if (queue->length == 1)
    {
        queue->head->next = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }

    printf("Enqueing Process Queue\n");
    queue->length++;
    table->size++;
}
PID32 PSMQDequeueProcess(void *self, PSMQProcTable *table, ProcessPriority priority, PID32 pid)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;

    if (priority < 0 || priority > HL_PROC_PRIORITY_VERY_HIGH)
    {
        return ERROR_PID;
    }

    PSMQQueue *queue = manager->table->queues[priority];
    if (queue == NULL || queue->head == NULL)
    {
        return ERROR_PID;
    }

    PSMQNode *prev = NULL;
    PSMQNode *current = queue->head;

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            if (getFromProcessArray(&manager->manager.processes, pid)->state != HL_PROC_TERMINATED)
            {
                return ERROR_PID;
            }

            if (prev == NULL)
            {
                queue->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            if (queue->tail == current)
            {
                queue->tail = prev;
            }

            free(current);

            queue->length--;
            table->size--;

            return pid;
        }

        prev = current;
        current = current->next;
    }

    return ERROR_PID;
}

PID32 PSMQDequeueProcessByIndex(void *self, PSMQProcTable *table, ProcessPriority priority, size_t index)
{
    PrioritySchedulingMultipleQueueProcessManager *manager = (PrioritySchedulingMultipleQueueProcessManager *)self;
    PSMQQueue *queue = manager->table->queues[priority];

    if (queue->length == 1)
    {
        PSMQNode *node = queue->head;
        queue->head = NULL;
        queue->tail = NULL;

        PID32 toReturnPid = node->pid;

        FREE(node);

        return toReturnPid;
    }
    else if (queue->length > 1)
    {
        PSMQNode *previous = NULL;
        PSMQNode *current = queue->head;

        for (size_t i = 0; i < queue->length; i++)
        {
            if (i == index)
            {
                queue->length--;

                if (i == 0)
                {
                    queue->head = current->next;
                    PID32 toReturnPid = current->pid;
                    FREE(current);

                    return toReturnPid;
                }
                else if (i == queue->length - 1)
                {
                    queue->tail = previous;
                    queue->tail = NULL;

                    PID32 toReturnPid = current->pid;
                    FREE(current);

                    return toReturnPid;
                }
                else
                {
                    previous->next = current->next;

                    PID32 toReturnPid = current->pid;
                    FREE(current);

                    return toReturnPid;
                }
            }

            previous = current;
            current = current->next;
        }
    }

    return ERROR_PID;
}
