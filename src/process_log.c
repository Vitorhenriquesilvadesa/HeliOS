#include "process_log.h"
#include <stdio.h>

void writeLog(const char *algorithName, const char *fileName, ProcessLog *process, size_t logCount, const char *executionOrder)
{
    FILE *file = fopen(fileName, "a");

    if (!file)
    {
        perror("Erro ao abrir arquivo");
        return;
    }
    fprintf(file, algorithName);

    fprintf(file, "\nOrdem de Execução: %s\n\n", executionOrder);

    fprintf(file, "Processo\tTempo de Espera\t\tTempo de Retorno\n");

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (size_t i = 0; i < logCount; i++)
    {
        fprintf(file, "%s\t\t%d\t\t\t\t\t%d\n",
                process[i].name,
                process[i].waitingTime,
                process[i].turnaroundTime);

        totalWaitingTime += process[i].waitingTime;
        totalTurnaroundTime += process[i].turnaroundTime;
    }

    fprintf(file, "\nTempo Médio de Espera: %.2f\n", totalWaitingTime / logCount);
    fprintf(file, "Tempo Médio de Retorno: %.2f\n", totalTurnaroundTime / logCount);
    fprintf(file, "-------------------------------------------------------------\n");
    fclose(file);
};