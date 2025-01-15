/**
 * @file process.h
 * @brief Estruturas e funções para gerenciamento de processos no sistema operacional.
 *
 * Este arquivo define as estruturas e funções necessárias para a criação, manipulação e gerenciamento
 * de processos no sistema operacional. Os processos possuem um identificador único (PID), tempo de chegada,
 * tempo de execução, prioridade e outros atributos essenciais para o controle do sistema.
 */

#ifndef HELIOS_PROCESS_H
#define HELIOS_PROCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <instruction.h>
#include <system_types.h>
#include <program.h>

#define ERROR_PID -1 ///< Valor de erro para PID não encontrado

/**
 * @brief Estados possíveis de um processo.
 *
 * Enumeração que define os estados possíveis de um processo no sistema.
 */
typedef enum
{
    HL_PROC_READY,      ///< Processo está pronto para ser executado.
    HL_PROC_EXECUTING,  ///< Processo está em execução.
    HL_PROC_WAITING,    ///< Processo está aguardando algum recurso ou evento.
    HL_PROC_TERMINATED, ///< Processo terminou sua execução.
} ProcessState;

/**
 * @brief Prioridades de um processo.
 *
 * Enumeração que define as prioridades possíveis de um processo.
 */
typedef enum ProcessPriority
{
    HL_PROC_PRIORITY_DEFAULT = 0, ///< Prioridade padrão.
    HL_PROC_PRIORITY_VERY_LOW,    ///< Prioridade muito baixa.
    HL_PROC_PRIORITY_LOW,         ///< Prioridade baixa.
    HL_PROC_PRIORITY_MEDIUM,      ///< Prioridade média.
    HL_PROC_PRIORITY_HIGH,        ///< Prioridade alta.
    HL_PROC_PRIORITY_VERY_HIGH,   ///< Prioridade muito alta.
} ProcessPriority;

/**
 * @brief Estrutura que representa um processo no sistema.
 *
 * A estrutura Process contém todos os dados necessários para a execução de um processo no sistema,
 * incluindo seu identificador, estado, tempos de execução e as informações sobre o programa a ser executado.
 */
typedef struct Process
{
    PID32 pid;                  ///< Identificador único do processo.
    const char *name;           ///< Nome do processo.
    uint32_t arrivalTime;       ///< Tempo de chegada do processo.
    uint32_t burstTime;         ///< Tempo de execução do processo.
    int priority;               ///< Prioridade do processo.
    int pc;                     ///< Contador de programa (program counter).
    int sp;                     ///< Ponteiro de pilha.
    int accumulator;            ///< Acumulador para resultados temporários.
    Program *program;           ///< Ponteiro para o programa associado ao processo.
    ProcessState state;         ///< Estado atual do processo.
    Byte stack[STACK_MAX_SIZE]; ///< Pilha do processo.
    Byte registers[REG_MAX];    ///< Registradores do processo.
} Process;

/**
 * @brief Estrutura de informações para criação de um novo processo.
 *
 * Contém os parâmetros necessários para criar um processo no sistema operacional.
 */
typedef struct
{
    const char *name;         ///< Nome do processo.
    Time arrivalTime;         ///< Tempo de chegada do processo.
    int burstTime;            ///< Tempo de execução do processo.
    ProcessPriority priority; ///< Prioridade do processo.
    Program *program;         ///< Ponteiro para o programa associado ao processo.
} ProcessCreateInfo;

/**
 * @brief Estrutura que representa uma lista de processos.
 *
 * A estrutura ProcessArray é usada para armazenar múltiplos processos no sistema, permitindo
 * operações de gerenciamento e manipulação de processos, como inserção e remoção.
 */
typedef struct
{
    Process **processes; ///< Array de ponteiros para processos.
    size_t length;       ///< Quantidade de processos no array.
    size_t capacity;     ///< Capacidade do array de processos.
} ProcessArray;

/**
 * @brief Função para criar um novo processo.
 *
 * Cria um novo processo no sistema com base nas informações fornecidas.
 *
 * @param info Estrutura contendo as informações necessárias para a criação do processo.
 * @return Process* Ponteiro para o processo criado.
 */
Process *newProcess(ProcessCreateInfo info);

/**
 * @brief Função para inicializar um array de processos.
 *
 * Inicializa um array de processos com capacidade definida para armazenar os processos no sistema.
 *
 * @param array Ponteiro para o array de processos a ser inicializado.
 */
void initProcessArray(ProcessArray *array);

/**
 * @brief Função para adicionar um processo ao array de processos.
 *
 * Adiciona uma referência a um processo ao array de processos. O processo não é copiado, apenas
 * o ponteiro é adicionado ao array.
 *
 * @param array Ponteiro para o array de processos.
 * @param process Ponteiro para o processo a ser adicionado.
 */
void appendProcessReferenceArray(ProcessArray *array, Process *process);

/**
 * @brief Função para remover um processo do array de processos.
 *
 * Remove um processo do array de processos baseado no PID do processo.
 *
 * @param array Ponteiro para o array de processos.
 * @param pid Identificador do processo a ser removido.
 */
void removeFromProcessArray(ProcessArray *array, PID32 pid);

/**
 * @brief Função para obter um processo do array de processos.
 *
 * Obtém um processo do array de processos baseado no PID.
 *
 * @param array Ponteiro para o array de processos.
 * @param pid Identificador do processo a ser obtido.
 * @return Process* Ponteiro para o processo obtido.
 */
Process *getFromProcessArray(ProcessArray *array, PID32 pid);

/**
 * @brief Função para verificar se o array de processos contém algum processo.
 *
 * Verifica se o array de processos contém pelo menos um processo.
 *
 * @param array Ponteiro para o array de processos.
 * @return bool Retorna verdadeiro se houver processos no array, falso caso contrário.
 */
bool hasProcess(ProcessArray *array);

/**
 * @brief Função para obter o tempo de burst de um processo.
 *
 * Retorna o tempo de execução (burst time) de um processo.
 *
 * @param process Ponteiro para o processo.
 * @return Time O tempo de execução do processo.
 */
Time getProcessBurstTime(Process *process);

#endif
