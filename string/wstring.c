//
// Created by unixeno on 2018/10/12.
//


#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "wstring.h"

#define STRING_SIZE_INCREMENT 16

static int8_t _wstring_space_check(WString wstring, size_t need);


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
    wstring->data  = malloc(STRING_SIZE_INCREMENT * sizeof(wchar_t));
    wstring->length = 0;
    wstring->size = STRING_SIZE_INCREMENT;
    return wstring;
}

WString wstring_create_from_int(int32_t data) {
    WString wstring = wstring_create();
    swprintf(wstring->data, STRING_SIZE_INCREMENT * sizeof(wchar_t), L"%d", data);
    wstring->length = wcslen(wstring->data);
    return wstring;
}

WString wstring_create_from_wstring(WString source)
{
    WString wstring = wstring_create();
    wstring_append_wstr(wstring, wstring_get_wstr(source));
    return wstring;
}

uint8_t wstring_append_wchar(WString wstring, wchar_t ch)
{
    if (_wstring_space_check(wstring, 1))
    {
        wstring->data[wstring->length++] = ch;
        wstring->data[wstring->length] = 0;
    }
    return 0;           // 追加失败
}

uint8_t wstring_append_wstr(WString wstring, const wchar_t *wstr)
{
    size_t str_len = wcslen(wstr);
    if (_wstring_space_check(wstring, str_len))
    {
        memcpy(wstring->data + wstring->length, wstr, (str_len + 1) * sizeof(wchar_t));
        wstring->length += str_len;
        return 1;
    }
    else
    {
        return 0;
    }
}

const wchar_t *wstring_get_wstr(WString wstring)
{
    return wstring->data;
}

int8_t _wstring_space_check(WString wstring, size_t need)
{
    if (wstring->length + sizeof(wchar_t) + need > wstring->size)
    {
        size_t new_size = wstring->size + need;
        new_size *= sizeof(wchar_t);
        void* new_space = realloc(wstring->data, new_size);
        if (new_space == NULL)
        {
            return 0;
        }
        else
        {
            wstring->data = new_space;
            wstring->size += need;
            return 2;
        }
    }
    return 1;
}
