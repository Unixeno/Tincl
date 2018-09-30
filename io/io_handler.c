//
// Created by unixeno on 2018/9/27.
//

#include "io_handler.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../stack/stack.h"
#include "buffer.h"
#include <string.h>

typedef struct
{
    BUFFER *buffer;
    uint32_t line;
    uint32_t column;
    char *filename;
}FILE_INFO;


struct
{
    char **opened_file_list;
    size_t file_amount;
    size_t maximum_amount;
}FILE_LIST;

static STACK FILE_INFO_STACK;

void _file_list_append(char *filename);

void io_handler_init()
{
    FILE_LIST.file_amount = 0;
    FILE_LIST.maximum_amount = 16;
    FILE_LIST.opened_file_list = malloc(sizeof(char*) * 16);
    if (FILE_LIST.opened_file_list == NULL)
    {
        fputs("can't malloc space for file list", stderr);
        exit(-1);
    }
    FILE_INFO_STACK = stack_init(sizeof(FILE_INFO));
    if (FILE_INFO_STACK == NULL)
    {
        fputs("can't create stack for file stack", stderr);
    }
}

void io_handler_open(char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "can't open file '%s'", filename);
        exit(-1);
    }
    size_t filename_length = strlen(filename) + 1;
    char *filename_cache = malloc(filename_length);
    memcpy(filename_cache, filename, filename_length);

    // append filename to file list
    _file_list_append(filename_cache);

    // create file info struct
    FILE_INFO *info = malloc(sizeof(FILE_INFO));
    if (info == NULL)
    {
        fprintf(stderr, "can't malloc space to store file info for file '%s'", filename);
        exit(-1);
    }
    info->line = 1;
    info->column = 0;
    info->filename = filename_cache;
    info->buffer = buffer_init(fp);

    stack_push(FILE_INFO_STACK, info);
}

void io_handler_getchar(CharStruct *ch)
{
    FILE_INFO *info = stack_top(FILE_INFO_STACK);
    if (info == NULL)
    {
        ch->ch = WEOF;
        ch->line = 0;
        ch->column = 0;
        ch->filename = "";
        return;
    }
    wchar_t tmp_ch;
    while (1)               // this loop while remove
    {
        tmp_ch = buffer_getchar(info->buffer);
        info->column ++;
        if (tmp_ch == L'\\')
        {
            if (buffer_getchar(info->buffer) == L'\n')
            {
                info->column = 0;
                info->line ++;
                continue;
            }
            else
                buffer_ungetchar(info->buffer);     // the next char is not \n so we should put it back
        }
        break;
    }
    if (tmp_ch == L'\n')
    {
        info->line ++;
        info->column = 0;
    }

    ch->column = info->column;
    ch->line = info->line;
    ch->ch = tmp_ch;
    ch->filename = info->filename;
}

void io_handler_ungetchar()
{
    FILE_INFO *info = stack_top(FILE_INFO_STACK);
    if (info == NULL)
    {
        return;
    }
    info->column--;
    buffer_ungetchar(info->buffer);
}

void _file_list_append(char *filename)
{
    if (FILE_LIST.file_amount == FILE_LIST.maximum_amount)
    {
        void *tmp = realloc(FILE_LIST.opened_file_list, FILE_LIST.maximum_amount + 16);
        if (tmp == NULL)
        {
            fputs("can't realloc space for file list", stderr);
            exit(-1);
        }
        FILE_LIST.opened_file_list = tmp;
        FILE_LIST.maximum_amount += 16;
    }
    FILE_LIST.opened_file_list[FILE_LIST.file_amount] = filename;
}

void io_handler_gettoken(wchar_t *token_string, size_t max_length)
{
    FILE_INFO *info = stack_top(FILE_INFO_STACK);
    if (info == NULL)
    {
        return;
    }
    buffer_fetchtoken(info->buffer, token_string);
}

void io_handler_reset()
{
    FILE_INFO *info = stack_top(FILE_INFO_STACK);
    if (info == NULL)
    {
        return;
    }
    buffer_reset(info->buffer);
}
