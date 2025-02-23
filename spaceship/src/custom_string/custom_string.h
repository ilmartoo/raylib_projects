#pragma once
#ifndef __ILMARTO_STRING_H_
#define __ILMARTO_STRING_H_

#include <stddef.h>

typedef struct String
{
    const char *const buffer;
    const size_t length;
} String;

/**
 * Creates a string from a null terminated string.
 * @param string Null terminated string.
 * @return Created string.
 */
String string_create(const char *string);
/**
 * Creates a new string from a substring of an already existing string.
 * @param string Original string.
 * @param start Start index of the new string (included). If `start >= string.length || start <= end`
 * then the new string will be empty.
 * @param end End index of the new string (excluded). If `end > string.length` then `string.length` will be used. If `end == 0 || end
 * <= start` then the new string will be empty.
 * @return Created substring.
 */
String string_slice(String string, size_t start, size_t end);
/**
 * Creates a new string concatenating two existing strings.
 * @param string_a String to put at the start.
 * @param string_b String to put at the end.
 * @param buffer Memory space where to store the string. Must be `sizeof(buffer) >= string_a.length + string_b.length
 * + 1`.
 * @return Concatenated string.
 */
String string_concat(String string_a, String string_b, char *const buffer);

/**
 * Converts a string to a null terminated string.
 * @param string String to convert.
 * @param buffer Memory space where to store the null terminated string. Must be `sizeof(buffer) >= string.length + 1`.
 * @return Null terminated string.
 */
char *string_extract(String string, char *buffer);

#endif // __ILMARTO_STRING_H_