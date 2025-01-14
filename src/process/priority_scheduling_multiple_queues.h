#ifndef HL_PRIORITY_SCHEDULING_MULTIPLE_QEUES_H
#define HL_PRIORITY_SCHEDULING_MULTIPLE_QEUES_H

#include <process_manager.h>
#include <system_types.h>

#define PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM 1

typedef struct PSMQNode
{
    struct PSMQNode *next;
    PID32 pid;
    ProcessPriority priority;
} PSMQNode;

typedef struct
{
    PSMQNode *head;
    PSMQNode *tail;
    Time quantum;
    size_t length;
} PSMQQueue;

typedef struct
{
    PSMQQueue *queues[HL_PROC_PRIORITY_VERY_HIGH + 1];
    Time quantums[HL_PROC_PRIORITY_VERY_HIGH + 1];
    ProcessPriority currentPriority;
    PSMQNode *currentNode;
    size_t currentIndex;
    size_t size;
} PSMQProcTable;

typedef struct
{
    ProcessManager manager;
    PSMQProcTable *table;
} PrioritySchedulingMultipleQueueProcessManager;

ProcessManager *createPrioritySchedulingMultipleQueueProcessManager(ProcessManagerCreateInfo createInfo);
PID32 PSMQCreateProcess(void *self, ProcessCreateInfo info);
PID32 PSMQScheduleProcess(void *self, void *procTable);
void *PSMQInitProcTable(void *self);
Process *PSMQGetProcess(void *self, PID32 pid);
bool PSMQHasProcess(void *self);
void PSMQOnProcessDetach(void *self, CpuInfo info);

void PSMQEnqueueProcess(void *self, PSMQProcTable *table, PID32 pid, ProcessPriority priority);
PID32 PSMQDequeueProcess(void *self, PSMQProcTable *table, ProcessPriority priority, PID32 pid);
PID32 PSMQDequeueProcessByIndex(void *self, PSMQProcTable *table, ProcessPriority priority, size_t index);

#endif