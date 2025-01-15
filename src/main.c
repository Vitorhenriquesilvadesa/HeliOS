#include <kernel.h>

Program *exampleProgram1()
{
    size_t count = 14;
    Byte *instructions = ALLOCATE(Byte, count);

    instructions[0] = HL_OP_PUSH;
    instructions[1] = 'H';
    instructions[2] = HL_OP_PUSH;
    instructions[3] = 'e';
    instructions[4] = HL_OP_PUSH;
    instructions[5] = 'l';
    instructions[6] = HL_OP_PUSH;
    instructions[7] = 'l';
    instructions[8] = HL_OP_PUSH;
    instructions[9] = 'o';
    instructions[10] = HL_OP_PUSH;
    instructions[11] = '\0';
    instructions[12] = HL_OP_PRINT;
    instructions[13] = HL_OP_HALT;

    ProgramCreateInfo createInfo = {
        .instructions = instructions,
        .count = count,
    };

    return createProgram(createInfo);
}

Program *exampleProgram2()
{
    size_t count = 15;
    Byte *instructions = ALLOCATE(Byte, count);

    instructions[0] = HL_OP_PUSH;
    instructions[1] = 'P';
    instructions[2] = HL_OP_PUSH;
    instructions[3] = 'r';
    instructions[4] = HL_OP_PUSH;
    instructions[5] = 'o';
    instructions[6] = HL_OP_PUSH;
    instructions[7] = 'c';
    instructions[8] = HL_OP_PUSH;
    instructions[9] = '1';
    instructions[10] = HL_OP_CREATE_PROCESS;
    instructions[11] = 1;
    instructions[12] = 0;
    instructions[13] = HL_PROC_PRIORITY_LOW;
    instructions[14] = HL_OP_HALT;

    static int i = 0;
    printf("%d\n", i++);

    ProgramCreateInfo createInfo = {
        .instructions = instructions,
        .count = count,
    };

    return createProgram(createInfo);
}

Program *exampleProgram3()
{
    size_t count = 32;

    Byte *instructions = ALLOCATE(Byte, count);

    instructions[0] = HL_OP_MOV_CONST;
    instructions[1] = REG_R0;
    instructions[2] = 1;
    instructions[3] = HL_OP_PUSH;
    instructions[4] = 'W';
    instructions[5] = HL_OP_PUSH;
    instructions[6] = 'o';
    instructions[7] = HL_OP_PUSH;
    instructions[8] = 'r';
    instructions[9] = HL_OP_PUSH;
    instructions[10] = 'l';
    instructions[11] = HL_OP_PUSH;
    instructions[12] = 'd';
    instructions[13] = HL_OP_PUSH;
    instructions[14] = '\n';
    instructions[15] = HL_OP_PUSH;
    instructions[16] = '\0';
    instructions[17] = HL_OP_PRINT;
    instructions[18] = 0;
    instructions[19] = HL_OP_MOV_CONST;
    instructions[20] = REG_R1;
    instructions[21] = 1;
    instructions[22] = HL_OP_ADD;
    instructions[23] = REG_R0;
    instructions[24] = REG_R1;
    instructions[25] = HL_OP_MOV_ACC;
    instructions[26] = REG_R0;
    instructions[27] = HL_OP_JNQ;
    instructions[28] = 3;
    instructions[29] = 6;
    instructions[30] = REG_R0;
    instructions[31] = HL_OP_HALT;

    ProgramCreateInfo createInfo = {
        .instructions = instructions,
        .count = count,
    };

    return createProgram(createInfo);
}

/*
 *
*   HL_PROC_MANAGER_TYPE_FIRST_COME_FIRST_SERVED,
    HL_PROC_MANAGER_TYPE_SHORTEST_JOB_FIRST,
    HL_PROC_MANAGER_TYPE_ROUND_ROBIN,
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_SINGLE_QUEUE,
    HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES,
    HL_PROC_MANAGER_TYPE_LOTTERY_SCHEDULING,
 *
 *
 */

const char* enumToString[HL_PROC_MANAGER_TYPE_MAX] ={
"First come first served",
    "Shortest Job First",
    "Round Robin",
    "Priority scheduling single queue",
    "Priority scheduling multiple queues",
    "Lotery scheduling"
};

int main(void)
{
    ProgramInstantiationFn *programs = ALLOCATE(ProgramInstantiationFn, 3);
    programs[0] = exampleProgram1;
    programs[1] = exampleProgram2;
    programs[2] = exampleProgram3;

    SystemCreateInfo createInfo = {
        .procManager = HL_PROC_MANAGER_TYPE_PRIORITY_SCHEDULING_MULTIPLE_QUEUES,
        .programs = programs,
    };

    createSystemInstance(createInfo);

    createProcessWithPriority("Test1", exampleProgram1, HL_PROC_PRIORITY_HIGH);

    // Nao rodar, é recursivo
    // createProcessWithPriority("Test2", exampleProgram2, HL_PROC_PRIORITY_MEDIUM);

    createProcessWithPriority("Test3", exampleProgram3, HL_PROC_PRIORITY_LOW);

    // runSystem();
    runSystemWithLog(enumToString[createInfo.procManager.type],"../logs/test123.txt");
    return 0;
}
