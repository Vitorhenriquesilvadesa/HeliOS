/**
 * @file lottery_scheduling.h
 * @brief Implementação do gerenciador de processos utilizando o algoritmo Lottery Scheduling.
 *
 * Este arquivo define as estruturas e funções necessárias para gerenciar processos
 * utilizando o algoritmo Lottery Scheduling, onde cada processo recebe uma quantidade de tickets
 * e o processo a ser executado é escolhido de forma probabilística com base nos tickets.
 */

#ifndef HL_LOTTERY_SCHEDULING_H
#define HL_LOTTERY_SCHEDULING_H

#include <process_manager.h>

/**
 * @brief Quantum de tempo para cada processo no algoritmo Lottery Scheduling.
 */
#define LOTTERY_QUANTUM 1

/**
 * @brief Representa um nó na lista de processos do Lottery Scheduling.
 *
 * @var LotteryNode::pid Identificador único do processo.
 * @var LotteryNode::tickets Número de tickets atribuídos ao processo.
 * @var LotteryNode::next Ponteiro para o próximo nó na lista.
 */
typedef struct LotteryNode
{
    PID32 pid;                ///< Identificador do processo.
    int tickets;              ///< Número de tickets do processo.
    struct LotteryNode *next; ///< Próximo nó na lista.
} LotteryNode;

/**
 * @brief Representa a tabela de processos para o algoritmo Lottery Scheduling.
 *
 * @var LotteryProcTable::head Ponteiro para o início da lista de processos.
 * @var LotteryProcTable::size Número total de processos na tabela.
 * @var LotteryProcTable::totalTickets Soma total de tickets na tabela.
 */
typedef struct
{
    LotteryNode *head; ///< Início da lista de processos.
    size_t size;       ///< Número de processos na tabela.
    int totalTickets;  ///< Soma total de tickets na tabela.
} LotteryProcTable;

/**
 * @brief Gerenciador de processos que utiliza o algoritmo Lottery Scheduling.
 *
 * @var LotterySchedulingProcessManager::manager Estrutura base do gerenciador de processos.
 * @var LotterySchedulingProcessManager::procTable Tabela de processos do Lottery Scheduling.
 */
typedef struct
{
    ProcessManager manager;      ///< Gerenciador de processos base.
    LotteryProcTable *procTable; ///< Tabela de processos do Lottery Scheduling.
} LotterySchedulingProcessManager;

/**
 * @brief Cria um gerenciador de processos Lottery Scheduling.
 *
 * @param createInfo Informações para inicialização do gerenciador de processos.
 * @return Ponteiro para o gerenciador de processos criado.
 */
ProcessManager *createLotterySchedulingProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo e o adiciona ao gerenciador Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param info Informações do processo a ser criado.
 * @return O identificador do processo criado.
 */
PID32 LSCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Agenda o próximo processo para execução com base nos tickets de loteria.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param procTable Ponteiro para a tabela de processos.
 * @return O identificador do processo agendado.
 */
PID32 LSScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos do Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @return Ponteiro para a tabela de processos inicializada.
 */
void *LSInitProcTable(void *self);

/**
 * @brief Obtém um processo na tabela de processos pelo PID.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param pid Identificador do processo a ser obtido.
 * @return Ponteiro para o processo correspondente.
 */
Process *LSGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na tabela de processos do Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @return `true` se houver processos na tabela; caso contrário, `false`.
 */
bool LSHasProcess(void *self);

/**
 * @brief Lida com a remoção de um processo da CPU e atualiza o gerenciador Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param info Informações sobre o estado da CPU ao destacar o processo.
 */
void LSOnProcessDetach(void *self, CpuInfo info);

/**
 * @brief Adiciona um processo à tabela de processos do Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param table Ponteiro para a tabela de processos.
 * @param pid Identificador do processo a ser adicionado.
 * @param tickets Número de tickets atribuídos ao processo.
 */
void LSEnqueueProcess(void *self, LotteryProcTable *table, PID32 pid, int tickets);

/**
 * @brief Remove o próximo processo da tabela de processos do Lottery Scheduling.
 *
 * @param self Ponteiro para o gerenciador de processos Lottery Scheduling.
 * @param table Ponteiro para a tabela de processos.
 * @return O identificador do processo removido.
 */
PID32 LSDequeueProcess(void *self, LotteryProcTable *table);

#endif
