#ifndef HL_FCFS_H
#define HL_FCFS_H

#include <process_manager.h>
#include <stdint.h>

typedef struct FCFSNode
{
    PID32 pid;
    struct FCFSNode *next;
} FCFSNode;

typedef struct
{
    FCFSNode *head;
    FCFSNode *tail;
    size_t size;
} FCFSProcTable;

typedef struct
{
    ProcessManager manager;
    FCFSProcTable *procTable;
} FirstComeFirstServedProcessManager;

ProcessManager *createFirstComeFirstServedProcessManager(ProcessManagerCreateInfo createInfo);
PID32 FCFSCreateProcess(void *self, const char *name, Program *program);
PID32 FCFSScheduleProcess(void *self, void *procTable);
void *FCFSInitProcTable(void *self);
Process *FCFSGetProcess(void *self, PID32 pid);
bool FCFSHasProcess(void *self);
void FCFSOnProcessDetach(void *self, CpuInfo info);

void FCFSEnqueueProcess(void *self, FCFSProcTable *table, PID32 pid);
PID32 FCFSDequeueProcess(void *self, FCFSProcTable *table);

#endif