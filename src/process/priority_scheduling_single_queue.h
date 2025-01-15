/**
 * @file priority_scheduling_single_queue.h
 * @brief Implementação do gerenciador de processos utilizando o algoritmo de Escalonamento por Prioridade com uma única fila.
 *
 * Este arquivo define as estruturas e funções necessárias para gerenciar processos utilizando o algoritmo de escalonamento por prioridade,
 * onde todos os processos são alocados em uma única fila e escalonados conforme a prioridade.
 */

#ifndef HL_PRIORITY_SCHEDULING_H
#define HL_PRIORITY_SCHEDULING_H

#include <process_manager.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Representa um nó na fila de processos de Escalonamento por Prioridade.
 *
 * @var PriorityNode::pid Identificador único do processo.
 * @var PriorityNode::next Ponteiro para o próximo nó na fila.
 */
typedef struct PriorityNode
{
    PID32 pid;                 ///< Identificador do processo.
    struct PriorityNode *next; ///< Ponteiro para o próximo nó na fila.
} PriorityNode;

/**
 * @brief Representa a tabela de processos para o algoritmo de Escalonamento por Prioridade com uma única fila.
 *
 * @var PriorityProcTable::head Ponteiro para o primeiro nó da fila.
 * @var PriorityProcTable::tail Ponteiro para o último nó da fila.
 * @var PriorityProcTable::size Número total de processos na fila.
 */
typedef struct
{
    PriorityNode *head; ///< Início da fila de processos.
    PriorityNode *tail; ///< Fim da fila de processos.
    size_t size;        ///< Número total de processos na fila.
} PriorityProcTable;

/**
 * @brief Gerenciador de processos utilizando o algoritmo de Escalonamento por Prioridade com uma única fila.
 *
 * @var PrioritySchedulingSingleQueueProcessManager::manager Estrutura base do gerenciador de processos.
 * @var PrioritySchedulingSingleQueueProcessManager::procTable Tabela de processos que gerencia a fila de prioridade.
 */
typedef struct
{
    ProcessManager manager;       ///< Gerenciador de processos base.
    PriorityProcTable *procTable; ///< Tabela de processos com a fila de prioridade.
} PrioritySchedulingSingleQueueProcessManager;

/**
 * @brief Cria um gerenciador de processos Escalonamento por Prioridade com uma única fila.
 *
 * @param createInfo Informações para inicialização do gerenciador de processos.
 * @return Ponteiro para o gerenciador de processos criado.
 */
ProcessManager *createPrioritySchedulingSingleQueueProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo e o adiciona ao gerenciador Escalonamento por Prioridade com uma única fila.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param info Informações do processo a ser criado.
 * @return O identificador do processo criado.
 */
PID32 PSSQCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Agenda o próximo processo para execução na fila de prioridade.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param procTable Ponteiro para a tabela de processos.
 * @return O identificador do processo agendado.
 */
PID32 PSSQScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos do Escalonamento por Prioridade com uma única fila.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @return Ponteiro para a tabela de processos inicializada.
 */
void *PSSQInitProcTable(void *self);

/**
 * @brief Obtém um processo na tabela de processos pelo PID.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param pid Identificador do processo a ser obtido.
 * @return Ponteiro para o processo correspondente.
 */
Process *PSSQGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na tabela de processos do Escalonamento por Prioridade com uma única fila.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @return `true` se houver processos na tabela; caso contrário, `false`.
 */
bool PSSQHasProcess(void *self);

/**
 * @brief Lida com a remoção de um processo da CPU e atualiza o gerenciador Escalonamento por Prioridade com uma única fila.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param info Informações sobre o estado da CPU ao destacar o processo.
 */
void PSSQOnProcessDetach(void *self, CpuInfo info);

/**
 * @brief Adiciona um processo à fila de prioridade.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param table Ponteiro para a tabela de processos.
 * @param pid Identificador do processo a ser adicionado.
 */
void PSSQEnqueueProcess(void *self, PriorityProcTable *table, PID32 pid);

/**
 * @brief Remove o próximo processo da fila de prioridade.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com uma única fila.
 * @param table Ponteiro para a tabela de processos.
 * @return O identificador do processo removido.
 */
PID32 PSSQDequeueProcess(void *self, PriorityProcTable *table);

#endif
