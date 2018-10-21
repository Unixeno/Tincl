//
// Created by unixeno on 2018/9/29.
//

#ifndef C_BUFFER_H
#define C_BUFFER_H

#include <stdint.h>
#include <stdio.h>

#define BUFFER_BLOCK_SIZE 4

typedef struct BUFFER_BLOCK BUFFER_BLOCK;

typedef struct
{
    BUFFER_BLOCK *current_block;
    BUFFER_BLOCK *first_block;
    size_t current_pos;
    size_t begin_pos;
    FILE *input_file;
} BUFFER;

BUFFER *buffer_init(FILE *input_file);
wchar_t buffer_getchar(BUFFER *buffer);
int buffer_ungetchar(BUFFER *buffer);
uint8_t buffer_fetchtoken(BUFFER *buffer, wchar_t *token);
void buffer_free(BUFFER *buffer);
void buffer_reset(BUFFER *buffer);
#endif //C_BUFFER_H
