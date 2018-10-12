//
// Created by unixeno on 2018/10/12.
//


#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "wstring.h"

#define STRING_SIZE_INCREMENT 16

int8_t _string_space_check(WString wstring, size_t need);


void wstring_delete(WString wstring)
{
    if (wstring != NULL)
    {
        if (wstring->data != NULL)
        {
            free(wstring->data);
        }
        free(wstring);
    }
}

WString wstring_create()
{
    WString wstring = malloc(sizeof(_wstring_base));
    wstring->data  = malloc(STRING_SIZE_INCREMENT);
    wstring->length = 0;
    wstring->size = STRING_SIZE_INCREMENT;
}

WString wstring_create_from_int(int32_t data) {
    WString wstring = wstring_create();
    swprintf(wstring->data, STRING_SIZE_INCREMENT, L"%d", data);
    wstring->length = wcslen(wstring->data);
}

uint8_t wstring_append_char(WString wstring, wchar_t ch)
{
    if (_string_space_check(wstring, 1))
    {
        wstring->data[wstring->length++] = ch;
        wstring->data[wstring->length] = 0;
    }
    return 0;           // 追加失败
}

uint8_t wstring_append_cstr(WString wstring, wchar_t* wstr)
{
    size_t str_len = wcslen(wstr);
    if (_string_space_check(wstring, str_len))
    {
        memcpy(wstring->data, wstr, str_len + 1);
        return 1;
    }
    else
    {
        return 0;
    }
}

const wchar_t *wstring_get_cstr(WString wstring)
{
    return wstring->data;
}

int8_t _wstring_space_check(WString wstring, size_t need)
{
    if (wstring->length + 1 + need > wstring->size)
    {
        size_t new_size = wstring->size + (need / STRING_SIZE_INCREMENT * STRING_SIZE_INCREMENT + 1);
        void* new_space = realloc(wstring->data, new_size);
        if (new_space == NULL)
        {
            return 0;
        }
        else
        {
            wstring->data = new_space;
            wstring->size = new_size;
            return 2;
        }
    }
    return 1;
}