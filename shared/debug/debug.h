#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

// Debug functions //
#ifdef DEBUG

#include <stdio.h>

#define STR_ENDL  "\n"
#define STR_ERRO  "[ERRO] "
#define STR_WARN  "[WARN] "
#define STR_INFO  "[INFO] "
#define FILE_LINE "[%s:%d] "

// Logs an error with printf syntax
#define log_error(format, ...) fprintf(stdout, FILE_LINE STR_ERRO format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
// Logs a warning with printf syntax
#define log_warn(format, ...) fprintf(stdout, FILE_LINE STR_WARN format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)
// Logs information with printf syntax
#define log_info(format, ...) fprintf(stdout, FILE_LINE STR_INFO format STR_ENDL, __FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

// Logs an integer variable
#define log_int(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %d" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs an float variable
#define log_float(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %f" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a char variable
#define log_char(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %c" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a string variable
#define log_string(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %s" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs an hexadecimal variable
#define log_hex(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %#X" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a pointer variable
#define log_pointer(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %p" STR_ENDL, __FILE__, __LINE__, #x, x)

// States a block of code
#define IF_DEBUG if (1)

#else

// Logs an error with printf syntax
#define log_error(format, ...)
// Logs a warning with printf syntax
#define log_warn(format, ...)
// Logs information with printf syntax
#define log_info(format, ...)

// Logs an integer variable
#define log_int(x)
// Logs an float variable
#define log_float(x)
// Logs a char variable
#define log_char(x)
// Logs a string variable
#define log_string(x)
// Logs an hexadecimal variable
#define log_hex(x)
// Logs a pointer variable
#define log_pointer(x)

// States a block of code
#define IF_DEBUG if (0)

#endif  // DEBUG

#endif  // __DEBUG_H__