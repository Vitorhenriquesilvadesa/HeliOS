/**
 * @file process_info.h
 * @brief Definição da estrutura para armazenar as estatísticas de processos.
 *
 * Este arquivo define a estrutura que contém as estatísticas associadas a um processo,
 * como tempos de chegada, início, término, espera e tempo de retorno. Essas informações
 * são úteis para análise e monitoramento do desempenho do escalonamento de processos.
 */

#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdint.h>
#include <system_types.h>

/**
 * @brief Estrutura que armazena as estatísticas de um processo.
 *
 * Esta estrutura contém informações sobre o processo, como o PID, nome, tempos de chegada,
 * início, término, tempos de espera e tempo de retorno. Essas métricas são úteis para análise
 * do desempenho do escalonamento de processos.
 *
 * @var ProcessStatistics::pid Identificador único do processo.
 * @var ProcessStatistics::name Nome do processo.
 * @var ProcessStatistics::arrivalTime Tempo em que o processo chegou ao sistema.
 * @var ProcessStatistics::startTime Tempo em que o processo começou a ser executado.
 * @var ProcessStatistics::endTime Tempo em que o processo terminou sua execução.
 * @var ProcessStatistics::waitingTime Tempo que o processo ficou esperando na fila.
 * @var ProcessStatistics::turnaroundTime Tempo total entre a chegada e a finalização do processo.
 */
typedef struct
{
    PID32 pid;               ///< Identificador único do processo.
    const char *name;        ///< Nome do processo.
    uint32_t arrivalTime;    ///< Tempo de chegada do processo ao sistema.
    uint32_t startTime;      ///< Tempo de início da execução do processo.
    uint32_t endTime;        ///< Tempo de término da execução do processo.
    uint32_t waitingTime;    ///< Tempo que o processo aguardou na fila.
    uint32_t turnaroundTime; ///< Tempo total entre a chegada e a finalização do processo.
} ProcessStatistics;

#endif
