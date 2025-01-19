/**
 * @file round_robin.h
 * @brief Estruturas e funções para o gerenciamento de processos com escalonamento Round Robin.
 *
 * Este arquivo define as estruturas e funções necessárias para implementar o escalonamento Round Robin,
 * um algoritmo de escalonamento simples e amplamente utilizado, onde cada processo é alocado uma fatia de tempo
 * (quantum) para execução de forma cíclica.
 */

#ifndef HL_ROUND_ROBIN_H
#define HL_ROUND_ROBIN_H

#include <process_manager.h>
#include <stdint.h>

#define ROUND_ROBIN_QUANTUM 1 ///< Definindo o quantum de tempo para o escalonamento Round Robin.

/**
 * @brief Estrutura que representa um nó da lista encadeada de processos no escalonamento Round Robin.
 *
 * Cada nó na lista representa um processo que está esperando para ser executado.
 * Ele contém o PID do processo e um ponteiro para o próximo nó na lista.
 */
typedef struct RRNode
{
    PID32 pid;           ///< PID do processo.
    struct RRNode *next; ///< Ponteiro para o próximo nó na lista.
} RRNode;

/**
 * @brief Estrutura que mantém a tabela de processos para o escalonamento Round Robin.
 *
 * A tabela contém uma lista de processos prontos para execução. Também mantém o controle do processo
 * atualmente em execução (head, tail, current) e o tamanho da fila.
 */
typedef struct
{
    RRNode *head;    ///< Ponteiro para o primeiro nó da lista de processos.
    RRNode *tail;    ///< Ponteiro para o último nó da lista de processos.
    RRNode *current; ///< Ponteiro para o nó que está sendo executado atualmente.
    size_t size;     ///< Tamanho da tabela de processos.
} RRProcTable;

/**
 * @brief Estrutura que representa o gerenciador de processos para o escalonamento Round Robin.
 *
 * Esta estrutura contém o gerenciador de processos, bem como a tabela de processos para o algoritmo Round Robin.
 */
typedef struct
{
    ProcessManager manager; ///< Gerenciador de processos base.
    RRProcTable *procTable; ///< Tabela de processos específica para o Round Robin.
} RoundRobinProcessManager;

/**
 * @brief Cria um gerenciador de processos para o escalonamento Round Robin.
 *
 * Função que cria e inicializa um novo gerenciador de processos para o algoritmo Round Robin, configurando
 * o quantum de tempo e a tabela de processos.
 *
 * @param createInfo Informações para criar o gerenciador de processos.
 * @return ProcessManager* Ponteiro para o gerenciador de processos recém-criado.
 */
ProcessManager *createRoundRobinProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Cria um novo processo para o escalonamento Round Robin.
 *
 * Função que cria um novo processo e o adiciona à tabela de processos Round Robin.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Informações necessárias para a criação do processo.
 * @return PID32 ID do processo recém-criado.
 */
PID32 RRCreateProcess(void *self, ProcessCreateInfo info);

/**
 * @brief Realiza o escalonamento de um processo no algoritmo Round Robin.
 *
 * Função que seleciona o próximo processo da tabela de processos para execução, aplicando o algoritmo
 * de escalonamento Round Robin.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param procTable Tabela de processos para o Round Robin.
 * @return PID32 ID do processo selecionado para execução.
 */
PID32 RRScheduleProcess(void *self, void *procTable);

/**
 * @brief Inicializa a tabela de processos para o Round Robin.
 *
 * Função que cria e inicializa a tabela de processos, configurando os ponteiros e a estrutura de dados
 * necessária para o escalonamento Round Robin.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @return void* Ponteiro para a tabela de processos recém-inicializada.
 */
void *RRInitProcTable(void *self);

/**
 * @brief Obtém o processo de um determinado PID da tabela de processos Round Robin.
 *
 * Função que busca um processo específico na tabela de processos com base no PID fornecido.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param pid PID do processo a ser obtido.
 * @return Process* Ponteiro para o processo correspondente ao PID.
 */
Process *RRGetProcess(void *self, PID32 pid);

/**
 * @brief Verifica se há processos na tabela de processos Round Robin.
 *
 * Função que verifica se existem processos prontos para execução na tabela de processos Round Robin.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @return bool Retorna verdadeiro se houver processos prontos, falso caso contrário.
 */
bool RRHasProcess(void *self);

/**
 * @brief Função chamada quando um processo é desalocado ou removido.
 *
 * Função que é chamada para realizar operações de limpeza ou atualizações quando um processo é removido
 * ou desalocado da tabela de processos Round Robin.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Informações do CPU associadas ao processo a ser removido.
 */
void RROnProcessDetach(void *self, CpuInfo info);

#endif
