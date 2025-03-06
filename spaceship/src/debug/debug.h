#pragma once
#ifndef __ILMARTO_DEBUG_H_
#define __ILMARTO_DEBUG_H_

// Debug functions //
#ifdef DEBUG

#include <stdio.h>

#include "raylib.h"

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

// Logs a boolean variable
#define log_bool(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %s" STR_ENDL, __FILE__, __LINE__, #x, x ? "true" : "false")
// Logs an integer variable
#define log_int(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %d" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a float variable
#define log_float(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %f" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a char variable
#define log_char(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %c" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a string variable
#define log_string(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %s" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a hexadecimal variable
#define log_hex(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %#X" STR_ENDL, __FILE__, __LINE__, #x, x)
// Logs a pointer variable
#define log_pointer(x) fprintf(stdout, FILE_LINE STR_INFO "%s = %p" STR_ENDL, __FILE__, __LINE__, #x, x)

// Logs a vector2 variable
#define log_vector2(x) fprintf(stdout, FILE_LINE STR_INFO "%s = (%.3f, %.3f)" STR_ENDL, __FILE__, __LINE__, #x, x.x, x.y)

#else

// Logs an error with printf syntax
#define log_error(format, ...)
// Logs a warning with printf syntax
#define log_warn(format, ...)
// Logs information with printf syntax
#define log_info(format, ...)

// Logs a boolean variable
#define log_bool(x)
// Logs an integer variable
#define log_int(x)
// Logs a float variable
#define log_float(x)
// Logs a char variable
#define log_char(x)
// Logs a string variable
#define log_string(x)
// Logs a hexadecimal variable
#define log_hex(x)
// Logs a pointer variable
#define log_pointer(x)

// Logs a vector2 variable
#define log_vector2(x)

#endif // DEBUG

#endif // __ILMARTO_DEBUG_H_