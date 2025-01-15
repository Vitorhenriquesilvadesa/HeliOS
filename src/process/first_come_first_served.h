/**
 * @file first_come_first_served.h
 * @brief Implementação do gerenciador de processos utilizando o algoritmo First-Come, First-Served (FCFS).
 *
 * Este arquivo define as estruturas e funções necessárias para gerenciar processos
 * utilizando o algoritmo FCFS, no qual os processos são atendidos na ordem de chegada.
 */

#ifndef HL_FCFS_H
#define HL_FCFS_H

#include <process_manager.h>
#include <stdint.h>

/**
 * @brief Representa um nó na fila de processos FCFS.
 *
 * @var FCFSNode::pid Identificador único do processo.
 * @var FCFSNode::next Ponteiro para o próximo nó na fila.
 */
typedef struct FCFSNode
{
    PID32 pid;             ///< Identificador do processo.
    struct FCFSNode *next; ///< Próximo nó na fila.
} FCFSNode;

/**
 * @brief Representa a tabela de processos para o algoritmo FCFS.
 *
 * @var FCFSProcTable::head Ponteiro para o início da fila.
 * @var FCFSProcTable::tail Ponteiro para o final da fila.
 * @var FCFSProcTable::size Número total de processos na fila.
 */
typedef struct
{
    FCFSNode *head; ///< Início da fila.
    FCFSNode *tail; ///< Final da fila.
    size_t size;    ///< Número de processos na fila.
} FCFSProcTable;

/**
 * @brief Gerenciador de processos que utiliza o algoritmo FCFS.
 *
 * @var FirstComeFirstServedProcessManager::manager Estrutura base do gerenciador de processos.
 * @var FirstComeFirstServedProcessManager::procTable Tabela de processos FCFS.
 */
typedef struct
{
    ProcessManager manager;   ///< Gerenciador de processos base.
    FCFSProcTable *procTable; ///< Tabela de processos FCFS.
} FirstComeFirstServedProcessManager;

/**
 * @brief Cria um gerenciador de processos FCFS.
 *
 * @param createInfo Informações para inicialização do gerenciador de processos.
 * @return Ponteiro para o gerenciador de processos criado.
 */
ProcessManager *createFirstComeFirstServedProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo e o adiciona ao gerenciador FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param info Informações do processo a ser criado.
 * @return O identificador do processo criado.
 */
PID32 FCFSCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Agenda o próximo processo na fila para execução.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param procTable Ponteiro para a tabela de processos.
 * @return O identificador do processo agendado.
 */
PID32 FCFSScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @return Ponteiro para a tabela de processos inicializada.
 */
void *FCFSInitProcTable(void *self);

/**
 * @brief Obtém um processo na tabela de processos pelo PID.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param pid Identificador do processo a ser obtido.
 * @return Ponteiro para o processo correspondente.
 */
Process *FCFSGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na fila de processos FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @return `true` se houver processos na fila; caso contrário, `false`.
 */
bool FCFSHasProcess(void *self);

/**
 * @brief Lida com a remoção de um processo da CPU e atualiza o gerenciador FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param info Informações sobre o estado da CPU ao destacar o processo.
 */
void FCFSOnProcessDetach(void *self, CpuInfo info);

/**
 * @brief Adiciona um processo à tabela de processos FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param table Ponteiro para a tabela de processos.
 * @param pid Identificador do processo a ser adicionado.
 */
void FCFSEnqueueProcess(void *self, FCFSProcTable *table, PID32 pid);

/**
 * @brief Remove o próximo processo da tabela de processos FCFS.
 *
 * @param self Ponteiro para o gerenciador de processos FCFS.
 * @param table Ponteiro para a tabela de processos.
 * @return O identificador do processo removido.
 */
PID32 FCFSDequeueProcess(void *self, FCFSProcTable *table);

#endif
