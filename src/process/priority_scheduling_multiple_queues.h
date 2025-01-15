/**
 * @file priority_scheduling_multiple_queues.h
 * @brief Implementação do gerenciador de processos utilizando o algoritmo de Escalonamento por Prioridade com Múltiplas Filas.
 *
 * Este arquivo define as estruturas e funções necessárias para gerenciar processos utilizando o algoritmo de escalonamento por prioridade,
 * onde processos com diferentes prioridades são alocados em filas separadas e escalonados conforme sua prioridade.
 */

#ifndef HL_PRIORITY_SCHEDULING_MULTIPLE_QEUES_H
#define HL_PRIORITY_SCHEDULING_MULTIPLE_QEUES_H

#include <process_manager.h>
#include <system_types.h>

/**
 * @brief Quantum de tempo para cada processo nas filas de prioridade no algoritmo de Escalonamento por Prioridade com Múltiplas Filas.
 */
#define PRIORITY_SCHEDULING_MULTIPLE_QEUES_QUANTUM 5

/**
 * @brief Representa um nó na fila de processos de Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @var PSMQNode::next Ponteiro para o próximo nó na fila.
 * @var PSMQNode::pid Identificador único do processo.
 * @var PSMQNode::priority Prioridade do processo.
 */
typedef struct PSMQNode
{
    struct PSMQNode *next;    ///< Ponteiro para o próximo nó na fila.
    PID32 pid;                ///< Identificador do processo.
    ProcessPriority priority; ///< Prioridade do processo.
} PSMQNode;

/**
 * @brief Representa uma fila de processos para um nível de prioridade no algoritmo de Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @var PSMQQueue::head Ponteiro para o primeiro nó da fila.
 * @var PSMQQueue::tail Ponteiro para o último nó da fila.
 * @var PSMQQueue::quantum Quantum de tempo para os processos da fila.
 * @var PSMQQueue::length Número de processos na fila.
 */
typedef struct
{
    PSMQNode *head; ///< Início da fila de processos.
    PSMQNode *tail; ///< Fim da fila de processos.
    Time quantum;   ///< Quantum de tempo para os processos dessa fila.
    size_t length;  ///< Número de processos na fila.
} PSMQQueue;

/**
 * @brief Tabela de processos para o algoritmo de Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @var PSMQProcTable::queues Array de filas de processos, uma para cada nível de prioridade.
 * @var PSMQProcTable::quantums Array de quantums de tempo para cada nível de prioridade.
 * @var PSMQProcTable::currentPriority Prioridade atual da fila de processos a ser escalonada.
 * @var PSMQProcTable::currentNode Nó atual da fila de processos.
 * @var PSMQProcTable::currentIndex Índice atual da fila de prioridade sendo escalonada.
 * @var PSMQProcTable::size Número total de processos na tabela.
 */
typedef struct
{
    PSMQQueue *queues[HL_PROC_PRIORITY_VERY_HIGH + 1]; ///< Filas de processos por prioridade.
    Time quantums[HL_PROC_PRIORITY_VERY_HIGH + 1];     ///< Quantums de tempo por prioridade.
    ProcessPriority currentPriority;                   ///< Prioridade atual da fila.
    PSMQNode *currentNode;                             ///< Nó atual sendo escalonado.
    size_t currentIndex;                               ///< Índice da fila de prioridade atual.
    size_t size;                                       ///< Número total de processos.
} PSMQProcTable;

/**
 * @brief Gerenciador de processos que utiliza o algoritmo de Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @var PrioritySchedulingMultipleQueueProcessManager::manager Estrutura base do gerenciador de processos.
 * @var PrioritySchedulingMultipleQueueProcessManager::table Tabela de processos que gerencia as filas.
 */
typedef struct
{
    ProcessManager manager; ///< Gerenciador de processos base.
    PSMQProcTable *table;   ///< Tabela de processos com as filas de prioridade.
} PrioritySchedulingMultipleQueueProcessManager;

/**
 * @brief Cria um gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @param createInfo Informações para inicialização do gerenciador de processos.
 * @return Ponteiro para o gerenciador de processos criado.
 */
ProcessManager *createPrioritySchedulingMultipleQueueProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo e o adiciona ao gerenciador Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param info Informações do processo a ser criado.
 * @return O identificador do processo criado.
 */
PID32 PSMQCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Agenda o próximo processo para execução com base na prioridade e no quantum de tempo.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param procTable Ponteiro para a tabela de processos.
 * @return O identificador do processo agendado.
 */
PID32 PSMQScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos do Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @return Ponteiro para a tabela de processos inicializada.
 */
void *PSMQInitProcTable(void *self);

/**
 * @brief Obtém um processo na tabela de processos pelo PID.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param pid Identificador do processo a ser obtido.
 * @return Ponteiro para o processo correspondente.
 */
Process *PSMQGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na tabela de processos do Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @return `true` se houver processos na tabela; caso contrário, `false`.
 */
bool PSMQHasProcess(void *self);

/**
 * @brief Lida com a remoção de um processo da CPU e atualiza o gerenciador Escalonamento por Prioridade com Múltiplas Filas.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param info Informações sobre o estado da CPU ao destacar o processo.
 */
void PSMQOnProcessDetach(void *self, CpuInfo info);

/**
 * @brief Adiciona um processo à fila de prioridade correspondente.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param table Ponteiro para a tabela de processos.
 * @param pid Identificador do processo a ser adicionado.
 * @param priority Prioridade do processo a ser adicionado.
 */
void PSMQEnqueueProcess(void *self, PSMQProcTable *table, PID32 pid, ProcessPriority priority);

/**
 * @brief Remove o próximo processo da fila de prioridade correspondente.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param table Ponteiro para a tabela de processos.
 * @param priority Prioridade da fila de processos.
 * @param pid Identificador do processo a ser removido.
 * @return O identificador do processo removido.
 */
PID32 PSMQDequeueProcess(void *self, PSMQProcTable *table, ProcessPriority priority, PID32 pid);

/**
 * @brief Remove um processo da fila de prioridade pelo índice.
 *
 * @param self Ponteiro para o gerenciador de processos Escalonamento por Prioridade com Múltiplas Filas.
 * @param table Ponteiro para a tabela de processos.
 * @param priority Prioridade da fila de processos.
 * @param index Índice do processo na fila.
 * @return O identificador do processo removido.
 */
PID32 PSMQDequeueProcessByIndex(void *self, PSMQProcTable *table, ProcessPriority priority, size_t index);

#endif
