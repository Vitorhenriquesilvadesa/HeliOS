#ifndef HL_SYSTEM_TYPES_H
#define HL_SYSTEM_TYPES_H

/**
 * @file system_types.h
 * @brief Define tipos básicos utilizados no sistema virtual HeliOS.
 */

/**
 * @typedef PID32
 * @brief Representa um identificador único para processos no sistema.
 *
 * Este tipo é usado para identificar processos dentro do sistema operacional.
 */
typedef int PID32;

/**
 * @typedef Time
 * @brief Representa o tempo em unidades específicas do sistema.
 *
 * Este tipo pode ser usado para medir intervalos de tempo ou agendamento de processos.
 */
typedef unsigned int Time;

/**
 * @typedef Byte
 * @brief Representa um único byte de dados.
 *
 * Usado para operações que requerem manipulação de dados em nível de byte.
 */
typedef char Byte;

#endif
