/**
 * @file instruction.h
 * @brief Definições de instruções e registradores para o sistema virtual HeliOS.
 *
 * Este arquivo contém as definições das instruções e registradores utilizados
 * pela máquina virtual do sistema operacional HeliOS. Ele também define o tamanho
 * máximo da pilha do sistema.
 */

#ifndef HL_INSTRUCTION_H
#define HL_INSTRUCTION_H

/// Tamanho máximo da pilha do sistema.
#define STACK_MAX_SIZE 32

/**
 * @brief Enumeração de instruções suportadas pela máquina virtual.
 *
 * Essas instruções representam as operações básicas que a máquina virtual
 * pode executar.
 */
typedef enum
{
    HL_OP_HALT,           ///< Interrompe a execução do programa.
    HL_OP_ADD,            ///< Soma dois valores.
    HL_OP_SUB,            ///< Subtrai dois valores.
    HL_OP_MUL,            ///< Multiplica dois valores.
    HL_OP_DIV,            ///< Divide dois valores.
    HL_OP_MOV,            ///< Move um valor entre registradores.
    HL_OP_MOV_CONST,      ///< Move um valor constante para um registrador.
    HL_OP_MOV_ACC,        ///< Move o valor do acumulador para um registrador.
    HL_OP_PUSH,           ///< Empilha um valor na pilha.
    HL_OP_POP,            ///< Remove um valor do topo da pilha.
    HL_OP_GOTO,           ///< Salta para uma instrução específica.
    HL_OP_JEQ,            ///< Salta se os valores forem iguais.
    HL_OP_JNQ,            ///< Salta se os valores forem diferentes.
    HL_OP_PRINT,          ///< Imprime um valor.
    HL_OP_CREATE_PROCESS, ///< Cria um novo processo.
} Instruction;

/**
 * @brief Enumeração dos registradores da máquina virtual.
 *
 * Os registradores são utilizados para armazenar valores temporários durante
 * a execução do programa.
 */
typedef enum
{
    REG_R0, ///< Registrador 0.
    REG_R1, ///< Registrador 1.
    REG_R2, ///< Registrador 2.
    REG_R3, ///< Registrador 3.
    REG_R4, ///< Registrador 4.
    REG_R5, ///< Registrador 5.
    REG_R6, ///< Registrador 6.
    REG_R7, ///< Registrador 7.
    REG_S1, ///< Registrador especial 1.
    REG_MAX ///< Limite superior para registradores válidos.
} Register;

#endif
