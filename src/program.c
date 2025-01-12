#include <program.h>
#include <kernel.h>

Program *createProgram(ProgramCreateInfo createInfo)
{
    Program *program = ALLOCATE(Program, 1);

    program->instructions = createInfo.instructions;
    program->count = createInfo.count;

    return program;
}