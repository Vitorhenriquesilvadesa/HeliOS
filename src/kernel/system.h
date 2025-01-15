/**
 * @file system.h
 * @brief Definições e interfaces principais para o sistema operacional virtual HeliOS.
 *
 * Este arquivo contém as estruturas, tipos e funções essenciais para a criação e
 * gerenciamento do sistema operacional virtual HeliOS. Ele fornece suporte para
 * gerenciamento de processos, criação de sistemas e execução de programas associados.
 */

#ifndef HL_SYSTEM_H
#define HL_SYSTEM_H

#include <kernel.h>

/**
 * @brief Ponteiro de função que faz o papel do construtor dos Process Managers.
 *
 * @param createInfo Conjunto de especificações utilizadas no momento da criação
 * do gerenciador.
 *
 * @return Um ponteiro para o novo Process Manager criado.
 */
typedef ProcessManager *(ProcManagerCreationFn)(ProcessManagerCreateInfo createInfo);

/**
 * @brief Especifica os parâmetros para a criação do sistema virtual.
 *
 * @param procManager Diz o tipo de algoritmo de agendamento de processos que
 * será utilizado pelo sistema operacional.
 * @param programs Especifica os conjuntos de instruções disponíveis para os processos
 * executarem.
 */
typedef struct
{
    ProcessManagerCreateInfo procManager; ///< Configuração do gerenciador de processos.
    ProgramInstantiationFn *programs;     ///< Lista de programas disponíveis.
} SystemCreateInfo;

/**
 * @brief Representa o sistema operacional virtual HeliOS.
 *
 * @var HeliOSSystem::processManager Gerenciador de processos do sistema.
 * @var HeliOSSystem::programs Lista de programas disponíveis para execução.
 * @var HeliOSSystem::cpu Representa a CPU virtual associada ao sistema.
 * @var HeliOSSystem::processTable Tabela de processos atualmente em execução.
 */
typedef struct HeliOSSystem
{
    ProcessManager *processManager;
    ProgramInstantiationFn *programs;
    Cpu *cpu;
    void *processTable;
} HeliOSSystem;

/**
 * @brief Cria uma instância do sistema virtual.
 *
 * @param createInfo Informações de criação do sistema.
 */
void createSystemInstance(SystemCreateInfo createInfo);

/**
 * @brief Cria um novo processo.
 *
 * @param name Nome do processo.
 * @param program Programa associado ao processo.
 *
 * @return O identificador do processo criado (PID32).
 */
PID32 createProcess(const char *name, ProgramInstantiationFn program);

/**
 * @brief Cria um novo processo com prioridade especificada.
 *
 * @param name Nome do processo.
 * @param program Programa associado ao processo.
 * @param priority Prioridade atribuída ao processo.
 *
 * @return O identificador do processo criado (PID32).
 */
PID32 createProcessWithPriority(const char *name, ProgramInstantiationFn program, ProcessPriority priority);

/**
 * @brief Cria o gerenciador de processos especificado.
 *
 * @param createInfo Informações de criação do gerenciador.
 *
 * @return Um ponteiro para o gerenciador de processos criado.
 */
ProcessManager *createProcessManager(ProcessManagerCreateInfo createInfo);

/**
 * @brief Obtém o programa associado a um processo.
 *
 * @param pid Identificador do processo.
 *
 * @return O programa associado ao processo.
 */
Program getProcessProgram(PID32 pid);

/**
 * @brief Obtém um programa pela sua posição na lista.
 *
 * @param index Índice do programa na lista.
 *
 * @return O programa correspondente ao índice fornecido.
 */
ProgramInstantiationFn getProgramByIndex(Byte index);

/**
 * @brief Inicia a execução do sistema.
 */
void runSystem();

/**
 * @brief Libera a memória usada pela instância atual do sistema.
 */
void freeSystemInstance();

/**
 * @brief Inicia o sistema e registra um log em um arquivo.
 *
 * @param logFile Caminho para o arquivo de log.
 */
void runSystemWithLog(const char *logFile);

#endif
