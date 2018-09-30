//
// Created by unixeno on 2018/9/27.
//

#ifndef C_IO_HANDLER_H
#define C_IO_HANDLER_H

#include <stdio.h>
#include <stdint.h>

typedef struct
{
    wchar_t ch;
    uint32_t line;
    uint32_t column;
    char *filename;
}CharStruct;

void io_handler_init();
void io_handler_open(char* filename);
void io_handler_getchar(CharStruct *ch);
void io_handler_ungetchar();
void io_handler_gettoken(wchar_t *token_string, size_t max_length);
void io_handler_reset();
#endif //C_IO_HANDLER_H
