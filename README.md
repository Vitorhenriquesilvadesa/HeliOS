# **HeliOS - Agendador de Processos**

HeliOS é um projeto acadêmico desenvolvido para o curso de Sistemas Operacionais no curso de Ciência da Computação. O objetivo deste projeto é implementar um agendador de processos com diversos algoritmos de escalonamento, incluindo:

- **First-Come, First-Served (FCFS)**
- **Shortest Job First (SJF)**
- **Round Robin (RR)**
- **Escalonamento de Prioridade (com e sem múltiplas filas)**

Além disso, o sistema implementa a simulação de uma CPU virtual, permitindo testar o funcionamento de diferentes estratégias de agendamento em um ambiente controlado.

## **Índice**

- [Descrição do Projeto](#descrição-do-projeto)
- [Funcionalidades](#funcionalidades)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Compilação e Execução](#compilação-e-execução)

## **Descrição do Projeto**

HeliOS simula o comportamento de um sistema operacional no gerenciamento de processos. O agendador de processos pode ser configurado para utilizar diferentes algoritmos de escalonamento, permitindo observar como o comportamento dos processos muda conforme o algoritmo selecionado.

A implementação foi feita em C e é composta por módulos que simulam o escalonamento de processos, a criação e gerenciamento de filas de processos, e a execução de processos em uma CPU virtual.

## **Funcionalidades**

- **Algoritmos de Escalonamento:**
  - First-Come, First-Served (FCFS)
  - Shortest Job First (SJF)
  - Round Robin (RR)
  - Escalonamento de Prioridade (com e sem múltiplas filas)
  
- **CPU Virtual:**
  - Simulação de uma CPU virtual para executar os processos agendados.
  
- **Estrutura Modular:**
  - O código é modular, permitindo a fácil adição de novos algoritmos de escalonamento ou modificação das existentes.

## **Estrutura do Projeto**

O projeto é organizado da seguinte forma:
Estrutura do Projeto:

| Pasta/Arquivo                 | Descrição                                                            |
|-------------------------------|----------------------------------------------------------------------|
| `src/`                         | Código fonte do projeto                                               |
| `src/process_manager.c`        | Gerenciamento de processos                                            |
| `src/round_robin.c`            | Algoritmo Round Robin                                                 |
| `src/shortest_job_first.c`     | Algoritmo Shortest Job First                                          |
| `src/priority_scheduling.c`    | Algoritmo de Escalonamento por Prioridade                             |
| `src/lottery_scheduling.c`     | Algoritmo Lottery Scheduling                                          |
| `src/main.c`                   | Função principal e execução do agendador                              |
| `process/process_manager.h`    | Definições de gerenciamento de processos                              |
| `process/round_robin.h`        | Definições para Round Robin                                           |
| `process/shortest_job_first.h` | Definições para Shortest Job First                                    |
| `process/priority_scheduling.h`| Definições para Escalonamento por Prioridade                         |
| `process/lottery_scheduling.h` | Definições para Lottery Scheduling                                    |
| `docs/`                        | Documentação gerada com Doxygen                                       |
| `CMake`                     | CMakeLists.txt para facilitar a distribuição |
| `README.md`                    | Este arquivo                                                         |


## **Compilação e Execução**

Para compilar e executar o projeto, siga os seguintes passos:

1. Clone o repositório:

```shell
git clone https://github.com/Vitorhenriquesilvadesa/HeliOS.git cd HeliOS
```

2. Gere o arquivo do compilador de sua preferência usando o CMakeLists.txt (MSVC, GCC, CLANG)

3. Compile o projeto

4. Execute o programa passando o ID do algoritmo a ser executado

```shell
./HeliOS.exe algorithm_id
```