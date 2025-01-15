#ifndef HL_PROGRAM_H
#define HL_PROGRAM_H

#include <instruction.h>
#include <stddef.h>
#include <system_types.h>
#include <stdint.h>

typedef struct
{
    Byte *instructions;
    size_t count;
} ProgramCreateInfo;

typedef struct
{
    Byte *instructions;
    size_t count;
} Program;

typedef Program *(*ProgramInstantiationFn)();

Program *createProgram(ProgramCreateInfo createInfo);

#endif