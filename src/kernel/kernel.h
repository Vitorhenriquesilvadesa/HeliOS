/**
 * @file kernel.h
 * @brief Cabeçalhos e definições principais para o sistema virtual HeliOS.
 *
 * Este arquivo inclui as dependências essenciais e define constantes globais
 * utilizadas no sistema operacional virtual.
 */

#include <process.h>         ///< Manipulação de processos.
#include <cpu.h>             ///< Definições e operações relacionadas à CPU.
#include <process_manager.h> ///< Gerenciamento de processos.
#include <allocator.h>       ///< Gerenciamento de memória.
#include <system.h>          ///< Estruturas e funções principais do sistema.
#include <system_types.h>    ///< Tipos básicos usados pelo sistema.
#include <stdio.h>           ///< Entrada e saída padrão para depuração e logs.

/**
 * @def QUANTUM_NONE
 * @brief Define o valor padrão para desativar o quantum do escalonador.
 *
 * Usado para desativar a contagem de tempo de execução dos processos.
 */
#define QUANTUM_NONE 0

/**
 * @def MAX_PROCESS_COUNT
 * @brief Define o número máximo de processos que o sistema pode gerenciar.
 *
 * Limita a quantidade de processos simultâneos para 32.
 */
#define MAX_PROCESS_COUNT 32
