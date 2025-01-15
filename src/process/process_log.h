//
// Created by guilherme on 14/01/25.
//

#ifndef PROCESS_LOG_H
#define PROCESS_LOG_H
#include "process_info.h"
#include <stddef.h>
void writeLog(const char *algorithName,const char *fileName, ProcessLog *process,size_t logCount, const char *executionOrder);
#endif //PROCESS_LOG_H