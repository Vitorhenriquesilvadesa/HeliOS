#ifndef HELIOS_PROCESS_H
#define HELIOS_PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <instruction.h>
#include <system_types.h>
#include <program.h>

#define ERROR_PID -1

typedef enum
{
    HL_PROC_READY,
    HL_PROC_EXECUTING,
    HL_PROC_WAITING,
    HL_PROC_TERMINATED,
} ProcessState;

typedef enum
{
    HL_PROC_PRIORITY_DEFAULT = 0,
    HL_PROC_PRIORITY_VERY_LOW,
    HL_PROC_PRIORITY_LOW,
    HL_PROC_PRIORITY_MEDIUM,
    HL_PROC_PRIORITY_HIGH,
    HL_PROC_PRIORITY_VERY_HIGH,
} ProcessPriority;

typedef struct Process
{
    PID32 pid;
    const char *name;
    int arrivalTime;
    int burstTime;
    int priority;
    int pc;
    int sp;
    int accumulator;
    Program *program;
    ProcessState state;
    Byte stack[STACK_MAX_SIZE];
    Byte registers[REG_MAX];
} Process;

typedef struct
{
    const char *name;
    int arrivalTime;
    int burstTime;
    ProcessPriority priority;
} ProcessCreateInfo;

typedef struct
{
    Process **processes;
    size_t length;
    size_t capacity;
} ProcessArray;

Process *newProcess(ProcessCreateInfo info, Program *program);
void initProcessArray(ProcessArray *array);
void appendProcessReferenceArray(ProcessArray *array, Process *process);
void removeFromProcessArray(ProcessArray *array, PID32 pid);
Process *getFromProcessArray(ProcessArray *array, PID32 pid);
bool hasProcess(ProcessArray *array);

#endif