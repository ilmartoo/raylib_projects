#pragma once
#ifndef __ILMARTO_DEBUG_H_
#define __ILMARTO_DEBUG_H_

// Debug functions //
#ifdef DEBUG

#include <stdio.h>

#define STR_ENDL "\n"
#define STR_ERRO "[ERRO] "
#define STR_WARN "[WARN] "
#define STR_INFO "[INFO] "
#define FILE_LINE "[%s:%d] "

// Logs an error with printf syntax
#define log_error(format, ...) fprintf(stderr, FILE_LINE STR_ERRO format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
// Logs a warning with printf syntax
#define log_warn(format, ...) fprintf(stderr, FILE_LINE STR_WARN format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
// Logs information with printf syntax
#define log_info(format, ...) fprintf(stderr, FILE_LINE STR_INFO format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

// Logs an integer variable
#define log_int(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %d" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs an float variable
#define log_float(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %f" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a char variable
#define log_char(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %c" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a string variable
#define log_string(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %s" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs an hexadecimal variable
#define log_hex(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %#X" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a pointer variable
#define log_pointer(x) fprintf(stderr, FILE_LINE STR_INFO "%s = %p" STR_ENDL, __FILE__, __LINE__, #x, x)

// States a block of code
#define DEBUG_BLOCK(code_block) \
    do {                        \
        code_block              \
    } while (0)

#else

// Logs an error with printf syntax
#define log_error(format, ...) \
    do {                       \
    } while (0)
// Logs a warning with printf syntax
#define log_warn(format, ...) \
    do {                      \
    } while (0)
// Logs information with printf syntax
#define log_info(format, ...) \
    do {                      \
    } while (0)

// Logs an integer variable
#define log_int(x) \
    do {           \
    } while (0)
// Logs an float variable
#define log_float(x) \
    do {             \
    } while (0)
// Logs a char variable
#define log_char(x) \
    do {            \
    } while (0)
// Logs a string variable
#define log_string(x) \
    do {              \
    } while (0)
// Logs an hexadecimal variable
#define log_hex(x) \
    do {           \
    } while (0)
// Logs a pointer variable
#define log_pointer(x) \
    do {               \
    } while (0)

// States a block of code
#define DEBUG_BLOCK(code_block) \
    do {                        \
    } while (0)

#endif // DEBUG

#endif // __ILMARTO_DEBUG_H_