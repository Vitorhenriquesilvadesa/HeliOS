#ifndef HL_ROUND_ROBIN_H
#define HL_ROUND_ROBIN_H

#include <process_manager.h>
#include <stdint.h>

#define ROUND_ROBIN_QUANTUM 1

typedef struct RRNode
{
    PID32 pid;
    struct RRNode *next;
} RRNode;

typedef struct
{
    RRNode *head;
    RRNode *tail;
    RRNode *current;
    size_t size;
} RRProcTable;

typedef struct
{
    ProcessManager manager;
    RRProcTable *procTable;
} RoundRobinProcessManager;

ProcessManager *createRoundRobinProcessManager(ProcessManagerCreateInfo createInfo);
PID32 RRCreateProcess(void *self, const char *name, Program *program);
PID32 RRScheduleProcess(void *self, void *procTable);
void *RRInitProcTable(void *self);
Process *RRGetProcess(void *self, PID32 pid);
bool RRHasProcess(void *self);
void RROnProcessDetach(void *self, CpuInfo info);

#endif