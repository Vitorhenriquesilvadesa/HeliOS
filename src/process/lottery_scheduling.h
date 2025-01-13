#ifndef HL_LOTTERY_SCHEDULING_H
#define HL_LOTTERY_SCHEDULING_H

#include <process_manager.h>

#define LOTTERY_QUANTUM 2

typedef struct LotteryNode
{
    PID32 pid;
    int tickets;
    struct LotteryNode *next;
} LotteryNode;

typedef struct
{
    LotteryNode *head;
    size_t size;
    int totalTickets;
} LotteryProcTable;

typedef struct
{
    ProcessManager manager;
    LotteryProcTable *procTable;
} LotterySchedulingProcessManager;

ProcessManager *createLotterySchedulingProcessManager(ProcessManagerCreateInfo createInfo);
PID32 LSCreateProcess(void *self, const char *name, Program *program, ProcessPriority priority);
PID32 LSScheduleProcess(void *self, void *procTable);
void *LSInitProcTable(void *self);
Process *LSGetProcess(void *self, PID32 pid);
bool LSHasProcess(void *self);
void LSOnProcessDetach(void *self, CpuInfo info);

void LSEnqueueProcess(void *self, LotteryProcTable *table, PID32 pid, int tickets);
PID32 LSDequeueProcess(void *self, LotteryProcTable *table);

#endif
