#ifndef HL_PROC_MANAGER_H
#define HL_PROC_MANAGER_H

#include <process.h>

typedef struct CpuInfo CpuInfo;

typedef PID32 (*ProcCreateFn)(void *self, const char *name, Program *program);
typedef Process *(*GetProcessFn)(void *self, PID32 pid);
typedef PID32 (*ProcScheduleFn)(void *self, void *procTable);
typedef void *(*InitProcTableFn)(void *self);
typedef bool (*HasProcFn)(void *self);
typedef void (*OnProcDetachFn)(void *self, CpuInfo info);

typedef enum
{
    HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED,
    HL_PROC_MANAGER_TYPE_SHORTEST_JOB_FIRST,
    HL_PROC_MANAGER_TYPE_ROUND_ROBIN,
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_SINGLE_QUEUE,
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES,
    HL_PROC_MANAGER_TYPE_LOTTERY_SCHEDULING,
    HL_PROC_MANAGER_TYPE_MAX,
} ProcManagerType;

typedef enum
{
    PREEMPTION_NONE,
    PREEMPTION_QUANTUM
} PreemptionType;

typedef struct
{
    ProcManagerType type;
    Time quantum;
} ProcessManagerCreateInfo;

typedef struct
{
    ProcManagerType type;
    PreemptionType preemptionType;
    ProcCreateFn createProc;
    ProcScheduleFn schedule;
    InitProcTableFn initTable;
    GetProcessFn getProcess;
    ProcessArray processes;
    HasProcFn hasProcess;
    OnProcDetachFn onDetach;
    Time quantum;
} ProcessManager;

#endif