//
// Created by unixeno on 2018/10/12.
//

#include "error_handler.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_LEXICAL,
}ERROR_TYPE;

void error_print(Token token, const char* format, ...)
{
    char buffer[128];

    fprintf(stderr, "%s:%d:%d error: ", token.filename, token.line, token.column);

    va_list arg_list;
    va_start (arg_list, format);
    vsnprintf(buffer, 128, format, arg_list);
    va_end(arg_list);
    fputs(buffer, stderr);
    exit(-1);
}
