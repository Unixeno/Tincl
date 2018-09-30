//
// Created by unixeno on 2018/9/28.
//


#ifndef C_STACK_H
#define C_STACK_H

#include <stdint.h>

struct STACK_t;

typedef struct STACK_t *STACK;

#define STACK_DEFAULT_SIZE 16


STACK stack_init(size_t element_size);
int stack_empty(STACK stack);
int stack_push(STACK stack, const void *element);
int stack_pop(STACK stack, void *element);
void *stack_top(STACK stack);
int stack_gettop(STACK stack, void *element);
void stack_destroy(STACK stack);

#endif //C_STACK_H
