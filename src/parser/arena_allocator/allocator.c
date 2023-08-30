#include "allocator.h"
#include <stdlib.h>

int a_allocator_size;
char* a_allocator_start;
int a_allocator_offset;

void ainit()
{
    a_allocator_size = 1024 * 1024 * 4;
    a_allocator_start = malloc(a_allocator_size);
    a_allocator_offset = 0;
}

void* aalloc(int size)
{
    if(a_allocator_offset + size >= a_allocator_size)
    {
        a_allocator_size *= 2;
        a_allocator_start = realloc(a_allocator_start, a_allocator_size);
    }

    void* ret = a_allocator_start + a_allocator_offset;
    a_allocator_offset += size;
    return ret;
}

void aclose()
{
    free(a_allocator_start);
}