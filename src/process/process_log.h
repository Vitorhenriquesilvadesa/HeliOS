/**
 * @file process_log.h
 * @brief Funções para log de informações sobre a execução de processos.
 *
 * Este arquivo contém as definições das funções responsáveis por registrar informações
 * sobre a execução dos processos. O objetivo é permitir o acompanhamento do comportamento
 * dos processos, como o escalonamento e a execução de algoritmos.
 */

#ifndef PROCESS_LOG_H
#define PROCESS_LOG_H

#include "process_info.h"
#include <stddef.h>

/**
 * @brief Função para escrever logs de execução de processos.
 *
 * Esta função é responsável por escrever logs contendo informações sobre o comportamento
 * dos processos, como os tempos de execução, a ordem de execução e os algoritmos utilizados.
 * Esses logs são úteis para análise posterior da performance do sistema.
 *
 * @param algorithName Nome do algoritmo de escalonamento utilizado.
 * @param fileName Nome do arquivo onde o log será escrito.
 * @param process Array de estruturas ProcessStatistics contendo informações sobre os processos.
 * @param logCount Número de logs que serão gravados no arquivo.
 * @param executionOrder String que define a ordem de execução dos processos (ex: "FIFO", "SJF").
 *
 * @return void
 */
void writeLog(const char *algorithName, const char *fileName, ProcessStatistics *process, size_t logCount, const char *executionOrder);

#endif
