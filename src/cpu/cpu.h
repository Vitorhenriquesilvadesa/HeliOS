/**
 * @file cpu.h
 * @brief Definições e funções relacionadas à CPU virtual do sistema HeliOS.
 *
 * Este arquivo contém as estruturas e funções necessárias para gerenciar a CPU
 * virtual, incluindo execução de instruções, manipulação de registradores e
 * operações na pilha.
 */

#ifndef CPU_H
#define CPU_H

#include <instruction.h>
#include <program.h>
#include <process.h>
#include <system_types.h>

/**
 * @brief Representa o estado atual da CPU.
 */
typedef enum
{
    CPU_RUNNING, ///< A CPU está em execução.
    CPU_IDLE,    ///< A CPU está ociosa.
    CPU_HALTED,  ///< A CPU foi interrompida.
} CpuState;

/**
 * @brief Estrutura que define a CPU virtual do sistema.
 *
 * @var Cpu::accumulator Acumulador da CPU para operações aritméticas.
 * @var Cpu::pc Contador de programa (Program Counter).
 * @var Cpu::sp Ponteiro de pilha (Stack Pointer).
 * @var Cpu::currentProgram Programa atualmente em execução.
 * @var Cpu::state Estado atual da CPU.
 * @var Cpu::currentProcessId Identificador do processo atualmente em execução.
 * @var Cpu::registers Registradores gerais da CPU.
 * @var Cpu::stack Pilha da CPU.
 * @var Cpu::cycles Contador de ciclos de instrução executados.
 */
typedef struct
{
    Byte accumulator;
    Byte pc;
    Byte sp;
    Program currentProgram;
    CpuState state;
    PID32 currentProcessId;
    Byte registers[REG_MAX];
    Byte stack[STACK_MAX_SIZE];
    uint32_t cycles;
} Cpu;

/**
 * @brief Estrutura que contém informações resumidas sobre a CPU.
 *
 * Esta estrutura é usada ao destacar um processo da CPU.
 */
typedef struct CpuInfo
{
    Byte accumulator;           ///< Acumulador.
    Byte pc;                    ///< Contador de programa.
    Byte sp;                    ///< Ponteiro de pilha.
    Program currentProgram;     ///< Programa atual.
    CpuState state;             ///< Estado da CPU.
    PID32 currentProcessId;     ///< ID do processo atual.
    Byte registers[REG_MAX];    ///< Registradores.
    Byte stack[STACK_MAX_SIZE]; ///< Pilha da CPU.
} CpuInfo;

/**
 * @brief Inicializa a CPU.
 *
 * @param cpu Ponteiro para a estrutura da CPU a ser inicializada.
 */
void initCpu(Cpu *cpu);

/**
 * @brief Anexa um processo à CPU.
 *
 * @param cpu Ponteiro para a CPU.
 * @param process Processo a ser anexado.
 */
void attachProcess(Cpu *cpu, Process *process);

/**
 * @brief Remove o processo atualmente anexado à CPU e retorna suas informações.
 *
 * @param cpu Ponteiro para a CPU.
 * @return As informações do processo que foi destacado.
 */
CpuInfo detachProcess(Cpu *cpu);

/**
 * @brief Decodifica e executa a próxima instrução da CPU.
 *
 * @param cpu Ponteiro para a CPU.
 */
void decodeAndExecute(Cpu *cpu);

/**
 * @brief Lê um byte da memória.
 *
 * @param cpu Ponteiro para a CPU.
 * @return O byte lido.
 */
Byte readByte(Cpu *cpu);

/**
 * @brief Insere um valor constante na pilha da CPU.
 *
 * @param cpu Ponteiro para a CPU.
 * @param constant Valor constante a ser empilhado.
 */
void cpuPush(Cpu *cpu, int constant);

/**
 * @brief Remove um valor da pilha e o move para um registrador.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg Registrador de destino.
 */
void cpuPop(Cpu *cpu, Register reg);

/**
 * @brief Realiza a soma de dois registradores.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg1 Primeiro registrador.
 * @param reg2 Segundo registrador.
 */
void cpuAdd(Cpu *cpu, Register reg1, Register reg2);

/**
 * @brief Realiza a subtração entre dois registradores.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg1 Primeiro registrador.
 * @param reg2 Segundo registrador.
 */
void cpuSub(Cpu *cpu, Register reg1, Register reg2);

/**
 * @brief Multiplica os valores de dois registradores.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg1 Primeiro registrador.
 * @param reg2 Segundo registrador.
 */
void cpuMul(Cpu *cpu, Register reg1, Register reg2);

/**
 * @brief Divide os valores de dois registradores.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg1 Primeiro registrador.
 * @param reg2 Segundo registrador.
 */
void cpuDiv(Cpu *cpu, Register reg1, Register reg2);

/**
 * @brief Move um valor constante para um registrador.
 *
 * @param cpu Ponteiro para a CPU.
 * @param value Valor a ser movido.
 * @param reg1 Registrador de destino.
 */
void cpuMovConst(Cpu *cpu, Byte value, Register reg1);

/**
 * @brief Move o valor do acumulador para um registrador.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg1 Registrador de destino.
 */
void cpuMovAcc(Cpu *cpu, Register reg1);

/**
 * @brief Salta para uma instrução específica.
 *
 * @param cpu Ponteiro para a CPU.
 */
void cpuJump(Cpu *cpu);

/**
 * @brief Salta se o valor no registrador for igual ao valor especificado.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg Registrador a ser comparado.
 * @param value Valor a ser comparado.
 * @param index Índice de destino do salto.
 */
void cpuJumpIfEqual(Cpu *cpu, Register reg, Byte value, Byte index);

/**
 * @brief Salta se o valor no registrador for diferente do valor especificado.
 *
 * @param cpu Ponteiro para a CPU.
 * @param reg Registrador a ser comparado.
 * @param value Valor a ser comparado.
 * @param index Índice de destino do salto.
 */
void cpuJumpIfNotEqual(Cpu *cpu, Register reg, Byte value, Byte index);

/**
 * @brief Imprime o valor do acumulador.
 *
 * @param cpu Ponteiro para a CPU.
 */
void cpuPrint(Cpu *cpu);

/**
 * @brief Interrompe a execução da CPU.
 *
 * @param cpu Ponteiro para a CPU.
 */
void cpuHalt(Cpu *cpu);

/**
 * @brief Cria um novo processo com a prioridade especificada.
 *
 * @param cpu Ponteiro para a CPU.
 * @param priority Prioridade do processo.
 * @param nameIndex Índice do nome do processo.
 * @param programIndex Índice do programa associado.
 */
void cpuCreateProcess(Cpu *cpu, ProcessPriority priority, Byte nameIndex, Byte programIndex);

#endif
