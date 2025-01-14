#include <process.h>
#include <kernel.h>
#include <string.h>

Process *newProcess(ProcessCreateInfo info)
{
    Process *process = ALLOCATE(Process, 1);

    process->name = info.name;
    process->program = info.program;
    process->arrivalTime = info.arrivalTime;
    process->burstTime = info.burstTime;
    process->priority = info.priority;

    process->accumulator = 0;
    process->pc = 0;
    process->sp = 0;
    process->pid = ERROR_PID;
    process->state = HL_PROC_READY;

    memset(process->stack, 0, STACK_MAX_SIZE);
    memset(process->registers, 0, REG_MAX);

    return process;
}

void initProcessArray(ProcessArray *array)
{
    array->processes = ALLOCATE(Process *, ARRAY_MIN_SIZE);
    array->capacity = ARRAY_MIN_SIZE;
    array->length = 0;
}

void appendProcessReferenceArray(ProcessArray *array, Process *process)
{
    if (array->length >= array->capacity - 1)
    {
        array->processes = realloc(array->processes, array->capacity * ARRAY_GROW_FACTOR);
    }

    array->processes[array->length++] = process;
}

void removeFromProcessArray(ProcessArray *array, PID32 pid)
{
    for (size_t i = 0; i < array->length; i++)
    {
        if (array->processes[i]->pid == pid)
        {
            FREE(array->processes[i]);
            array->processes[i] = array->processes[array->length - 1];
            array->length--;
            break;
        }
    }
}

Process *getFromProcessArray(ProcessArray *array, PID32 pid)
{
    for (size_t i = 0; i < array->length; i++)
    {
        if (array->processes[i]->pid == pid)
        {
            return array->processes[i];
        }
    }

    return NULL;
}

bool hasProcess(ProcessArray *array)
{
    return array->length > 0;
}

Time getProcessBurstTime(Process *process)
{
    return process->burstTime;
}
