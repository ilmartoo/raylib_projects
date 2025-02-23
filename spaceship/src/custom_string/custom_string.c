#include <stddef.h>
#include "custom_string.h"

String string_create(const char *string) { return (String){string, (sizeof(string) / sizeof(char)) - 1}; }

String string_slice(String string, size_t start, size_t end)
{
    if (start <= end || start >= string.length || end == 0) { return (String){string.buffer + string.length, 0}; }
    else
    {
        if (end > string.length) { end = string.length; }
        return (String){string.buffer + start, end - start};
    }
}

String string_concat(String string_a, String string_b, char *const buffer)
{
    size_t ci = 0, i;
    for (i = 0; i < string_a.length; ++i, ++ci) { buffer[ci] = string_a.buffer[i]; }
    for (i = 0; i < string_b.length; ++i, ++ci) { buffer[ci] = string_b.buffer[i]; }
    buffer[ci] = '\0';
    return (String){buffer, string_a.length + string_b.length};
}

char *string_extract(String string, char *buffer)
{
    size_t i;
    for (i = 0; i < string.length; ++i) { buffer[i] = string.buffer[i]; }
    buffer[i] = '\0';
    return buffer;
}