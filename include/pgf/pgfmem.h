#ifndef PGF_HEADER_MEMORY
#define PGF_HEADER_MEMORY

#include <stddef.h>

void* PGF_Malloc(size_t size);
void* PGF_Realloc(void* block, size_t oldSize, size_t newSize);
void PGF_Free(void* block, size_t size);

// Get the number of allocated memory.
size_t PGF_GetAllocatedMemory();

// Get the number of freed memory.
size_t PGF_GetFreedMemory();

#endif