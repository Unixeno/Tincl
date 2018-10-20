cmake_minimum_required(VERSION 3.12)
project(tincl C)

include_directories(.)

set(CMAKE_C_STANDARD 99)
add_executable(tincl main.c string/string.c string/string.h lex/lex.c lex/lex.h io/io_handler.c io/io_handler.h stack/stack.c stack/stack.h io/buffer.c io/buffer.h io/error_handler.c io/error_handler.h string/wstring.c string/wstring.h)