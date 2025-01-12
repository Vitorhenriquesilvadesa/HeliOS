#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

#define ARRAY_MIN_SIZE 8
#define ARRAY_GROW_FACTOR 2

#define ALLOCATE(type, count) (type *)malloc(sizeof(type) * (count))
#define FREE(ptr) free(ptr)

#endif