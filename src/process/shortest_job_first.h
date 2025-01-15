/**
 * @file shortest_job_first.h
 * @brief Estruturas e funções para o gerenciamento de processos com escalonamento Shortest Job First (SJF).
 *
 * Este arquivo define as estruturas e funções necessárias para implementar o escalonamento Shortest Job First,
 * um algoritmo de escalonamento baseado no tempo de execução estimado dos processos, onde o processo com o
 * menor tempo de execução é escolhido para execução primeiro.
 */

#ifndef HL_SHORTEST_JOB_FIRST_H
#define HL_SHORTEST_JOB_FIRST_H

#include <process_manager.h>
#include <stdint.h>

/**
 * @brief Estrutura que representa um nó da lista encadeada de processos no escalonamento Shortest Job First.
 *
 * Cada nó na lista contém o PID de um processo e um ponteiro para o próximo nó na lista. A lista é ordenada de acordo
 * com o tempo de execução estimado (burst time) dos processos, com o processo de menor tempo de execução no início da lista.
 */
typedef struct SJFNode
{
    PID32 pid;            ///< PID do processo.
    struct SJFNode *next; ///< Ponteiro para o próximo nó na lista.
} SJFNode;

/**
 * @brief Estrutura que mantém a tabela de processos para o escalonamento Shortest Job First.
 *
 * A tabela contém uma lista de processos prontos para execução, ordenada de acordo com o tempo de execução estimado dos processos.
 * O processo com o menor tempo de execução é o primeiro na fila.
 */
typedef struct
{
    SJFNode *head; ///< Ponteiro para o primeiro nó da lista de processos.
    SJFNode *tail; ///< Ponteiro para o último nó da lista de processos.
    size_t size;   ///< Tamanho da tabela de processos.
} SJFProcTable;

/**
 * @brief Estrutura que representa o gerenciador de processos para o escalonamento Shortest Job First.
 *
 * Esta estrutura contém o gerenciador de processos base e a tabela de processos ordenada do algoritmo Shortest Job First.
 */
typedef struct
{
    ProcessManager manager;  ///< Gerenciador de processos base.
    SJFProcTable *procTable; ///< Tabela de processos específica para o Shortest Job First.
} ShortestJobFirstProcessManager;

/**
 * @brief Cria um gerenciador de processos para o escalonamento Shortest Job First.
 *
 * Função que cria e inicializa um novo gerenciador de processos para o algoritmo Shortest Job First.
 * A tabela de processos será configurada para ordenar os processos com base no tempo de execução estimado.
 *
 * @param createInfo Informações para criar o gerenciador de processos.
 * @return ProcessManager* Ponteiro para o gerenciador de processos recém-criado.
 */
ProcessManager *createShortestJobFirstProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo para o escalonamento Shortest Job First.
 *
 * Função que cria um novo processo e o adiciona à tabela de processos do Shortest Job First.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Informações necessárias para a criação do processo.
 * @return PID32 ID do processo recém-criado.
 */
PID32 SJFCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Realiza o escalonamento de um processo no algoritmo Shortest Job First.
 *
 * Função que seleciona o próximo processo da tabela de processos com o menor tempo de execução estimado,
 * de acordo com o algoritmo Shortest Job First.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param procTable Tabela de processos para o Shortest Job First.
 * @return PID32 ID do processo selecionado para execução.
 */
PID32 SJFScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos para o Shortest Job First.
 *
 * Função que cria e inicializa a tabela de processos, configurando os ponteiros e a estrutura de dados necessária
 * para o algoritmo Shortest Job First.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @return void* Ponteiro para a tabela de processos recém-inicializada.
 */
void *SJFInitProcTable(void *self);

/**
 * @brief Obtém o processo de um determinado PID da tabela de processos Shortest Job First.
 *
 * Função que busca um processo específico na tabela de processos com base no PID fornecido.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param pid PID do processo a ser obtido.
 * @return Process* Ponteiro para o processo correspondente ao PID.
 */
Process *SJFGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na tabela de processos Shortest Job First.
 *
 * Função que verifica se existem processos prontos para execução na tabela de processos do Shortest Job First.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @return bool Retorna verdadeiro se houver processos prontos, falso caso contrário.
 */
bool SJFHasProcess(void *self);

/**
 * @brief Função chamada quando um processo é desalocado ou removido.
 *
 * Função que é chamada para realizar operações de limpeza ou atualizações quando um processo é removido
 * ou desalocado da tabela de processos Shortest Job First.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Informações do CPU associadas ao processo a ser removido.
 */
void SJFOnProcessDetach(void *self, CpuInfo info);

/**
 * @brief Adiciona um processo à tabela de processos Shortest Job First.
 *
 * Função que insere um novo processo na tabela de processos, mantendo a ordem de acordo com o tempo de execução estimado.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param table Tabela de processos para o Shortest Job First.
 * @param pid PID do processo a ser enfileirado.
 */
void SJFEnqueueProcess(void *self, SJFProcTable *table, PID32 pid);

/**
 * @brief Remove o processo de menor tempo de execução da tabela de processos Shortest Job First.
 *
 * Função que remove e retorna o PID do processo com o menor tempo de execução estimado.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param table Tabela de processos para o Shortest Job First.
 * @return PID32 PID do processo removido.
 */
PID32 SJFDequeueProcess(void *self, SJFProcTable *table);

#endif
