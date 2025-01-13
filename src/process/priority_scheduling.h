#ifndef HL_PRIORITY_SCHEDULING_H
#define HL_PRIORITY_SCHEDULING_H

#include <process_manager.h>
#include <stdint.h>

typedef struct PriorityNode
{
    PID32 pid;
    struct PriorityNode *next;
} PriorityNode;

typedef struct
{
    PriorityNode *head;
    PriorityNode *tail;
    size_t size;
} PriorityProcTable;

typedef struct
{
    ProcessManager manager;
    PriorityProcTable *procTable;
} PrioritySchedulingSingleQueueProcessManager;

ProcessManager *createPrioritySchedulingSingleQueueProcessManager(ProcessManagerCreateInfo createInfo);
PID32 PSSQCreateProcess(void *self, const char *name, Program *program, ProcessPriority priority);
PID32 PSSQScheduleProcess(void *self, void *procTable);
void *PSSQInitProcTable(void *self);
Process *PSSQGetProcess(void *self, PID32 pid);
bool PSSQHasProcess(void *self);
void PSSQOnProcessDetach(void *self, CpuInfo info);

void PSSQEnqueueProcess(void *self, PriorityProcTable *table, PID32 pid);
PID32 PSSQDequeueProcess(void *self, PriorityProcTable *table);

#endif
