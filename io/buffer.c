//
// Created by unixeno on 2018/9/29.
//

#include "buffer.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct BUFFER_BLOCK
{
    struct BUFFER_BLOCK *next;
    struct BUFFER_BLOCK *prev;
    wchar_t buffer[BUFFER_BLOCK_SIZE];
};

BUFFER_BLOCK *buffer_readblock(FILE *input_file);
BUFFER_BLOCK *buffer_freeblock(BUFFER_BLOCK *block);

BUFFER *buffer_init(FILE *input_file)
{
    BUFFER *buffer = malloc(sizeof(BUFFER));
    if (buffer == NULL)
    {
        fputs("failed to malloc buffer", stderr);
        exit(-1);
    }

    BUFFER_BLOCK *block = buffer_readblock(input_file);

    if (block == NULL)
    {
        fputs("failed to malloc block", stderr);
        free(buffer);
        exit(-1);
    }
    block->prev = NULL;

    buffer->input_file = input_file;

    // now we only have one block, so they all point to the first block
    buffer->current_block = block;
    buffer->first_block = block;
    // don't forget to set the pos to zero
    buffer->begin_pos = 0;
    buffer->current_pos = 0;

    return buffer;
}

BUFFER_BLOCK *buffer_readblock(FILE *input_file)
{
    if (input_file == NULL)
    {
        fputs("no input file\n", stderr);
        return NULL;
    }
    BUFFER_BLOCK *block = malloc(sizeof(BUFFER_BLOCK));
    if (block == NULL)
    {
        return NULL;
    }
    block->next = NULL;
    for (int i = 0; i < BUFFER_BLOCK_SIZE; ++i)
    {
        int tmp = fgetc(input_file);
        if (tmp == EOF)
        {
            block->buffer[i] = WEOF;
            return block;
        }
        block->buffer[i] = (wchar_t) tmp;
    }
    return block;
}

wchar_t buffer_getchar(BUFFER *buffer)
{
    if (buffer == NULL || buffer->current_block == NULL)
    {
        return 0;
    }
    if (buffer->current_pos == BUFFER_BLOCK_SIZE)   // now we are at the end of this block
    {
        // check if there is an block exist
        if (buffer->current_block->next == NULL)
        {
            // so create a new block for reading
            BUFFER_BLOCK *new_block = buffer_readblock(buffer->input_file);
            if (new_block == NULL)
            {
                return WEOF;
            }
            buffer->current_block->next = new_block;
            new_block->prev = buffer->current_block;
            buffer->current_block = new_block;

            buffer->current_pos = 0;
        }
        else
        {
            // ok, there is a block, just repoint current block to it
            buffer->current_block = buffer->current_block->next;
            // and reset current pos to zero
            buffer->current_pos = 0;
        }
    }
    // now we can happily returning the character
    return buffer->current_block->buffer[buffer->current_pos++];
}


int buffer_ungetchar(BUFFER *buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }
    if ((buffer->current_block == buffer->first_block) &&
        (buffer->current_pos == buffer->begin_pos))     // we can't unget a char before read a char
    {
        return 0;
    }
    if (buffer->current_pos == 1)  // we are at the beginning of current block
    {
        // BE careful!!! the range of current position is in 1 ~ BUFFER_BLOCK_SIZE
        // because the pos is pointed to the next char that will be returned
        // so when current pos = 1, it means the last char that returned is the first char in this block
        // therefor we should return back to previous block
        buffer->current_block = buffer->current_block->prev;
        buffer->current_pos = BUFFER_BLOCK_SIZE;
        return 1;
    }
//    else if (buffer->current_pos == BUFFER_BLOCK_SIZE)      // now, we are at the end of this block
//    {
//        // so we should
//        buffer->current_block = buffer->current_block->prev;
//    }
    else
    {
        buffer->current_pos--;
        return 1;
    }
}

size_t buffer_fetchtoken(BUFFER *buffer, wchar_t *token)
{
//    size_t end_pos = buffer->current_pos - 1;
    size_t block_pos = buffer->begin_pos;
    size_t token_pos = 0;
    BUFFER_BLOCK *current_block = buffer->first_block;
    while (1)
    {
        if (block_pos == buffer->current_pos)
        {
            if (current_block == buffer->current_block)
                break;
        }
        if (block_pos == BUFFER_BLOCK_SIZE)             // now we are at the end of this block
        {                                               // so the next char is in the next block
            current_block = current_block->next;
            block_pos = 0;
        }
        token[token_pos] = current_block->buffer[block_pos];
        token_pos++;
        block_pos++;
    }
    token[token_pos] = 0;           // end

    // free unused blocks
    current_block = buffer->first_block;
    while (current_block != buffer->current_block)
    {
        current_block = buffer_freeblock(current_block);
    }
    buffer->first_block = current_block;
    buffer->begin_pos = buffer->current_pos;
    return token_pos;
}

BUFFER_BLOCK *buffer_freeblock(BUFFER_BLOCK *block)
{
    BUFFER_BLOCK *next = block->next;
    free(block);
    return next;
}

void buffer_free(BUFFER *buffer)
{
    BUFFER_BLOCK *block = buffer->first_block;
    while (block != NULL)
    {
        block = buffer_freeblock(block);
    }
    free(buffer);
}

void buffer_reset(BUFFER *buffer)
{
    if (buffer == NULL)
        return;
    BUFFER_BLOCK *block = buffer->first_block;
    while (block != buffer->current_block)
    {
        block = buffer_freeblock(block);            // free unused block
    }
    buffer->begin_pos = buffer->current_pos;
    buffer->first_block = buffer->current_block;

}
