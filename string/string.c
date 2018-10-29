//
// Created by unixeno on 2018/9/14.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string.h"

#define STRING_SIZE_INCREMENT 16

static int8_t _string_space_check(String string, size_t need);


void string_delete(String string)
{
    if (string != NULL)
    {
        if (string->data != NULL)
        {
            free(string->data);
        }
        free(string);
    }
}

String string_create()
{
    String string = malloc(sizeof(_string_base));
    string->data  = malloc(STRING_SIZE_INCREMENT);
    string->length = 0;
    string->size = STRING_SIZE_INCREMENT;
}

String string_create_from_int(int32_t data) {
    String string = string_create();
    sprintf(string->data, "%d", data);
    string->length = strlen(string->data);
}

uint8_t string_append_char(String string, char ch)
{
    if (_string_space_check(string, 1))
    {
        string->data[string->length++] = ch;
        string->data[string->length] = 0;
    }
    return 0;           // 追加失败
}

uint8_t string_append_cstr(String string, char* cstr)
{
    size_t str_len = strlen(cstr);
    if (_string_space_check(string, str_len))
    {
        memcpy(string->data, cstr, str_len + 1);
        return 1;
    }
    else
    {
        return 0;
    }
}

const char *string_get_cstr(String string)
{
    return string->data;
}

int8_t _string_space_check(String string, size_t need)
{
    if (string->length + 1 + need > string->size)
    {
        size_t new_size = string->size + (need / STRING_SIZE_INCREMENT * STRING_SIZE_INCREMENT + 1);
        void* new_space = realloc(string->data, new_size);
        if (new_space == NULL)
        {
            return 0;
        }
        else
        {
            string->data = new_space;
            string->size = new_size;
            return 2;
        }
    }
    return 1;
}