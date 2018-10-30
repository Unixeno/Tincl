//
// Created by unixeno on 2018/10/12.
//

#ifndef TINCL_WSTRING_H
#define TINCL_WSTRING_H

#include <stdint.h>

typedef struct
{
    wchar_t *data;
    size_t size;
    size_t length;
}_wstring_base, *WString;


void wstring_delete(WString wstring);
WString wstring_create();
WString wstring_create_from_int(int32_t data);
WString wstring_create_from_wstring(WString source);
uint8_t wstring_append_wchar(WString string, wchar_t wch);
uint8_t wstring_append_wstr(WString string, const wchar_t *wstr);
const wchar_t *wstring_get_wstr(WString string);
#endif //TINCL_WSTRING_H
