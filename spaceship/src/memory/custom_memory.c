#include <stddef.h>
#include "memory.h"

void *memory_copy(void *dest, void *const src, size_t bytes)
{
    for (char *d = dest, *s = src; bytes; --bytes, ++d, ++s) { *d = *s; }
    return dest;
}

void *memory_zero(void *memory, size_t bytes)
{
    for (char *m = memory; bytes; --bytes, ++m) { *m = 0; }
    return memory;
}
