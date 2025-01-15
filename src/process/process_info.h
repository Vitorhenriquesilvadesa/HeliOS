//
// Created by guilherme on 14/01/25.
//

#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H
#include <stdint.h>
#include <system_types.h>

typedef struct {
    PID32 pid;
    const char *name;
    uint32_t arrivalTime;
    uint32_t startTime;
    uint32_t endTime;
    uint32_t waitingTime;
    uint32_t turnaroundTime;
}ProcessLog;

#endif //PROCESS_INFO_H
