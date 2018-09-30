//
// Created by unixeno on 2018/9/14.
//

#ifndef C_STRING_H
#define C_STRING_H

#include <stdint.h>

typedef struct
{
    char *data;
    size_t size;
    size_t length;
}_string_base, *String;


void string_delete(String string);
String string_create();
String string_create_from_int(int32_t data);
uint8_t string_append_char(String string, char ch);
uint8_t string_append_cstr(String string, char* cstr);
const char *string_get_cstr(String string);
#endif //C_STRING_H
