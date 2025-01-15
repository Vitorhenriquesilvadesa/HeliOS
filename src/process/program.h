/**
 * @file program.h
 * @brief Estruturas e funções para o gerenciamento de programas.
 *
 * Este arquivo define as estruturas e funções necessárias para a criação e manipulação de programas.
 * Um programa é composto por um conjunto de instruções que serão executadas pelo processador.
 * A estrutura de um programa inclui a lista de instruções e o número total de instruções.
 */

#ifndef HL_PROGRAM_H
#define HL_PROGRAM_H

#include <instruction.h>
#include <stddef.h>
#include <system_types.h>
#include <stdint.h>

/**
 * @brief Estrutura de informações para a criação de um programa.
 *
 * Contém os parâmetros necessários para criar um programa, incluindo um ponteiro para as instruções
 * e o número total de instruções que o programa irá conter.
 */
typedef struct
{
    Byte *instructions; ///< Ponteiro para o array de instruções do programa.
    size_t count;       ///< Quantidade total de instruções no programa.
} ProgramCreateInfo;

/**
 * @brief Estrutura que representa um programa.
 *
 * A estrutura Program armazena o conjunto de instruções de um programa a ser executado.
 * Ela contém um array de bytes representando as instruções e a quantidade total de instruções.
 */
typedef struct
{
    Byte *instructions; ///< Ponteiro para o array de instruções do programa.
    size_t count;       ///< Quantidade total de instruções no programa.
} Program;

/**
 * @brief Função de instância para criar um programa.
 *
 * Função para criar um novo programa baseado em uma estrutura de informações fornecida.
 * A criação de um programa envolve a alocação de memória para suas instruções.
 *
 * @return Program* Ponteiro para o programa recém-criado.
 */
typedef Program *(*ProgramInstantiationFn)();

/**
 * @brief Função para criar um programa.
 *
 * Cria um novo programa com base nas informações fornecidas, incluindo as instruções e a quantidade
 * de instruções. A memória é alocada para armazenar as instruções do programa.
 *
 * @param createInfo Estrutura contendo as informações necessárias para criar o programa.
 * @return Program* Ponteiro para o programa criado.
 */
Program *createProgram(ProgramCreateInfo createInfo);

#endif
