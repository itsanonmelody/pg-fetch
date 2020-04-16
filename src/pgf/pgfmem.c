#include <pgf/pgfmem.h>

#include <stdlib.h>
#include <assert.h>

static size_t g_allocatedMemory = 0;
static size_t g_freedMemory     = 0;

void* PGF_Malloc(size_t size)
{
    void* memory = malloc(size);
    if (memory)
    {
        g_allocatedMemory += size;
    }

    return memory;
}

void* PGF_Realloc(void* block, size_t oldSize, size_t newSize)
{
    void* memory = realloc(block, newSize);
    if (memory)
    {
        g_freedMemory += oldSize;
        g_allocatedMemory += newSize;
    }

    return memory;
}

void PGF_Free(void* block, size_t size)
{
    free(block);
    g_freedMemory += size;
}

// Get the number of allocated memory.
size_t PGF_GetAllocatedMemory()
{
    return g_allocatedMemory;
}

// Get the number of freed memory.
size_t PGF_GetFreedMemory()
{
    return g_freedMemory;
}