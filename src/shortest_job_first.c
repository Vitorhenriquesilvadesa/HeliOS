#include <shortest_job_first.h>
#include <kernel.h>

ProcessManager *createShortestJobFirstProcessManager(ProcessManagerCreateInfo createInfo) {
    ShortestJobFirstProcessManager *sjf = ALLOCATE(ShortestJobFirstProcessManager, 1);
    sjf->manager.type = HL_PROC_MANAGER_TYPE_SHORTEST_JOB_FIRST;
    sjf->manager.createProc = SJFCreateProcess;
    sjf->manager.initTable = SJFInitProcTable;
    sjf->manager.schedule = SJFScheduleProcess;
    sjf->manager.getProcess = SJFGetProcess;
    sjf->manager.hasProcess = SJFHasProcess;
    sjf->manager.onDetach = SJFOnProcessDetach;
    sjf->procTable = NULL;
    sjf->manager.quantum = QUANTUM_NONE;
    sjf->manager.preemptionType = PREEMPTION_NONE;

    return (ProcessManager *) sjf;
}

PID32 SJFCreateProcess(void *self, ProcessCreateInfo info) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    SJFProcTable *table = manager->procTable;

    static PID32 nextPid = 1;

    PID32 pid = nextPid++;

    Process *process = newProcess(info);
    process->pid = pid;

    appendProcessReferenceArray(&manager->manager.processes, process);
    SJFEnqueueProcess(self, manager->procTable, pid);
}

PID32 SJFScheduleProcess(void *self, void *procTable) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    SJFProcTable *table = (SJFProcTable *) procTable;

    const PID32 pid = SJFDequeueProcess(self, table);

    if (pid != ERROR_PID) {
        Process *process = getFromProcessArray(&manager->manager.processes, pid);
        return pid;
    }

    return ERROR_PID;
}

void *SJFInitProcTable(void *self) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    SJFProcTable *table = ALLOCATE(SJFProcTable, 1);

    table->head = NULL;
    table->tail = NULL;
    table->size = 0;
    initProcessArray(&manager->manager.processes);

    manager->procTable = table;
    return (void *) table;
}

Process *SJFGetProcess(void *self, PID32 pid) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    return getFromProcessArray(&manager->manager.processes, pid);
}

bool SJFHasProcess(void *self) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    return manager->procTable->size > 0;
}

void SJFOnProcessDetach(void *self, CpuInfo info) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;

    if (getFromProcessArray(&manager->manager.processes, info.currentProcessId)->state == HL_PROC_TERMINATED) {
        removeFromProcessArray(&manager->manager.processes, info.currentProcessId);
    }
}

void SJFEnqueueProcess(void *self, SJFProcTable *table, PID32 pid) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;
    Process *process = getFromProcessArray(&manager->manager.processes, pid);

    SJFNode *node = ALLOCATE(SJFNode, 1);
    node->pid = pid;
    node->next = NULL;

    if (table->head == NULL) {
        table->head = node;
        table->tail = node;
    } else {
        SJFNode *current = table->head;
        SJFNode *previous = NULL;

        while (current != NULL && getProcessBurstTime(process) >= getProcessBurstTime(
                   getFromProcessArray(&manager->manager.processes, current->pid))) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            node->next = table->head;
            table->head = node;
        } else {
            previous->next = node;
            node->next = current;

            if (current == NULL) {
                table->tail = node;
            }
        }
    }

    table->size++;
}

PID32 SJFDequeueProcess(void *self, SJFProcTable *table) {
    ShortestJobFirstProcessManager *manager = (ShortestJobFirstProcessManager *) self;

    if (!table->head) {
        fprintf(stderr, "Erro: Nenhum processo na fila FCFS.\n");
        return ERROR_PID;
    }

    SJFNode *nodeToRemove = table->head;
    PID32 pid = nodeToRemove->pid;

    table->head = nodeToRemove->next;
    if (!table->head) {
        table->tail = NULL;
    }

    free(nodeToRemove);

    table->size--;
    return pid;
}
