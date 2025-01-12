#include <kernel.h>

Program *exampleProgram1()
{
    size_t count = 18;
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
    instructions[11] = ',';
    instructions[12] = HL_OP_PUSH;
    instructions[13] = ' ';
    instructions[14] = HL_OP_PUSH;
    instructions[15] = '\0';
    instructions[16] = HL_OP_PRINT;
    instructions[17] = HL_OP_HALT;

    ProgramCreateInfo createInfo = {
        .instructions = instructions,
        .count = count,
    };

    return createProgram(createInfo);
}

Program *exampleProgram2()
{
    Byte *instructions = ALLOCATE(Byte, 16);

    instructions[0] = HL_OP_PUSH;
    instructions[1] = 'W';
    instructions[2] = HL_OP_PUSH;
    instructions[3] = 'o';
    instructions[4] = HL_OP_PUSH;
    instructions[5] = 'r';
    instructions[6] = HL_OP_PUSH;
    instructions[7] = 'l';
    instructions[8] = HL_OP_PUSH;
    instructions[9] = 'd';
    instructions[10] = HL_OP_PUSH;
    instructions[11] = '\n';
    instructions[12] = HL_OP_PUSH;
    instructions[13] = '\0';
    instructions[14] = HL_OP_PRINT;
    instructions[15] = HL_OP_HALT;

    size_t count = 16;

    ProgramCreateInfo createInfo = {
        .instructions = instructions,
        .count = count,
    };

    return createProgram(createInfo);
}

int main(void)
{
    SystemCreateInfo createInfo = {
        .procManager = HL_PROC_MANAGER_TYPE_ROUND_ROBIN,
    };

    createSystemInstance(createInfo);

    createProcess("Test1", exampleProgram1);
    createProcess("Test2", exampleProgram2);

    runSystem();

    return 0;
}
