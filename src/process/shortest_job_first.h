#ifndef HL_SHORTEST_JOB_FIRST_H
#define HL_SHORTEST_JOB_FIRST_H

#include <process_manager.h>
#include <stdint.h>

typedef struct SJFNode
{
    PID32 pid;
    struct SJFNode *next;
} SJFNode;

typedef struct
{
    SJFNode *head;
    SJFNode *tail;
    size_t size;
} SJFProcTable;

typedef struct
{
    ProcessManager manager;
    SJFProcTable *procTable;
} ShortestJobFirstProcessManager;

ProcessManager *createShortestJobFirstProcessManager(ProcessManagerCreateInfo createInfo);
PID32 SJFCreateProcess(void *self, const char *name, Program *program);
PID32 SJFScheduleProcess(void *self, void *procTable);
void *SJFInitProcTable(void *self);
Process *SJFGetProcess(void *self, PID32 pid);
bool SJFHasProcess(void *self);
void SJFOnProcessDetach(void *self, CpuInfo info);

void SJFEnqueueProcess(void *self, SJFProcTable *table, PID32 pid);
PID32 SJFDequeueProcess(void *self, SJFProcTable *table);

#endif