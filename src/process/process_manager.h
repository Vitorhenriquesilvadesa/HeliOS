/**
 * @file proc_manager.h
 * @brief Gerenciador de processos para escalonamento e execução de processos.
 *
 * Este arquivo contém a definição do gerenciador de processos que lida com a criação, escalonamento e
 * gerenciamento de processos em sistemas operacionais. O gerenciador de processos pode utilizar
 * diferentes algoritmos de escalonamento como First Come First Served (FCFS), Round Robin,
 * Shortest Job First (SJF), e outros.
 */

#ifndef HL_PROC_MANAGER_H
#define HL_PROC_MANAGER_H

#include <process.h>

typedef struct CpuInfo CpuInfo;

/**
 * @brief Função para criar um processo.
 *
 * Função responsável por criar um processo dado um conjunto de informações de criação.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Estrutura contendo informações necessárias para criar o processo.
 *
 * @return PID32 Identificador do processo criado.
 */
typedef PID32 (*ProcCreateFn)(void *self, ProcessCreateInfo info);

/**
 * @brief Função para obter um processo pelo seu PID.
 *
 * Função responsável por retornar um processo dado seu identificador único (PID).
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param pid Identificador do processo a ser obtido.
 *
 * @return Process* Ponteiro para a estrutura do processo.
 */
typedef Process *(*GetProcessFn)(void *self, PID32 pid);

/**
 * @brief Função para agendar um processo.
 *
 * Função responsável por agendar a execução de um processo baseado no algoritmo de escalonamento.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param procTable Tabela de processos que deve ser considerada para o agendamento.
 *
 * @return PID32 Identificador do processo agendado.
 */
typedef PID32 (*ProcScheduleFn)(void *self, void *procTable);

/**
 * @brief Função para inicializar a tabela de processos.
 *
 * Função responsável por inicializar a tabela de processos, estrutura que armazena os processos
 * disponíveis para o escalonamento.
 *
 * @param self Ponteiro para o gerenciador de processos.
 *
 * @return void* Ponteiro para a tabela de processos inicializada.
 */
typedef void *(*InitProcTableFn)(void *self);

/**
 * @brief Função para verificar se há processos.
 *
 * Função responsável por verificar se existem processos na tabela.
 *
 * @param self Ponteiro para o gerenciador de processos.
 *
 * @return bool Retorna verdadeiro se houver processos, caso contrário, falso.
 */
typedef bool (*HasProcFn)(void *self);

/**
 * @brief Função a ser chamada quando um processo for desacoplado.
 *
 * Função responsável por lidar com a desvinculação de um processo, o que pode envolver
 * operações de limpeza ou atualização do estado do gerenciador.
 *
 * @param self Ponteiro para o gerenciador de processos.
 * @param info Informações sobre o CPU no momento da desvinculação.
 */
typedef void (*OnProcDetachFn)(void *self, CpuInfo info);

/**
 * @brief Tipos de gerenciadores de processos.
 *
 * Enumeração que define os diferentes tipos de algoritmos de escalonamento de processos.
 */
typedef enum
{
    HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED,             ///< First Come First Served (FCFS)
    HL_PROC_MANAGER_TYPE_SHORTEST_JOB_FIRST,                  ///< Shortest Job First (SJF)
    HL_PROC_MANAGER_TYPE_ROUND_ROBIN,                         ///< Round Robin
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_SINGLE_QUEUE,    ///< Priority Scheduling (Single Queue)
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES, ///< Priority Scheduling (Multiple Queues)
    HL_PROC_MANAGER_TYPE_LOTTERY_SCHEDULING,                  ///< Lottery Scheduling
    HL_PROC_MANAGER_TYPE_MAX,                                 ///< Tipo máximo de gerenciador
} ProcManagerType;

/**
 * @brief Tipos de preempção no gerenciador de processos.
 *
 * Enumeração que define os tipos de preempção aplicáveis ao escalonamento de processos.
 */
typedef enum
{
    PREEMPTION_NONE,     ///< Sem preempção
    PREEMPTION_QUANTUM,  ///< Preempção por quantum de tempo
    PREEMPTION_PRIORITY, ///< Preempção por prioridade
} PreemptionType;

/**
 * @brief Estrutura com informações para criar um gerenciador de processos.
 *
 * Estrutura que contém as informações necessárias para criar um gerenciador de processos,
 * como o tipo de algoritmo de escalonamento e o quantum de tempo (se aplicável).
 */
typedef struct
{
    ProcManagerType type; ///< Tipo de gerenciador de processos
    Time quantum;         ///< Quantum de tempo (aplicável para algoritmos como Round Robin)
} ProcessManagerCreateInfo;

/**
 * @brief Tipo para as flags de preempção.
 */
typedef unsigned int PreemptionFlag;

/**
 * @brief Estrutura do gerenciador de processos.
 *
 * Estrutura que define o gerenciador de processos, incluindo os algoritmos de escalonamento,
 * tabelas de processos, e as funções associadas à criação, agendamento e gerenciamento de processos.
 */
typedef struct
{
    ProcManagerType type;          ///< Tipo do gerenciador de processos
    PreemptionFlag preemptionType; ///< Tipo de preempção utilizado
    ProcCreateFn createProc;       ///< Função para criação de processos
    ProcScheduleFn schedule;       ///< Função para escalonamento de processos
    InitProcTableFn initTable;     ///< Função para inicialização da tabela de processos
    GetProcessFn getProcess;       ///< Função para obtenção de um processo
    ProcessArray processes;        ///< Array de processos gerenciados
    HasProcFn hasProcess;          ///< Função para verificar se há processos
    OnProcDetachFn onDetach;       ///< Função chamada quando um processo é desacoplado
    Time quantum;                  ///< Quantum de tempo para algoritmos como Round Robin
} ProcessManager;

#endif
