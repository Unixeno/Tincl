//
// Created by unixeno on 2018/9/28.
//

#include "stack.h"
#include <stdlib.h>
#include <string.h>

typedef struct STACK_t
{
    uint8_t *data;
    size_t size;
    size_t top;
    size_t element_size;
} STACK_t;

STACK stack_init(size_t element_size)
{
    STACK stack = malloc(sizeof(STACK_t));
    stack->element_size = element_size;
    stack->top = 0;
    stack->data = malloc(STACK_DEFAULT_SIZE * element_size);
    stack->size = STACK_DEFAULT_SIZE;

    return stack;
}

int stack_empty(STACK stack)
{
    if (stack->top == 0)
        return 1;
    else
        return 0;
}

int stack_push(STACK stack, const void *element)
{
    if (stack->top == stack->size)      // not enough space
    {
        return 0;
    }
    else
    {
        memcpy(stack->data + stack->element_size * stack->top, element, stack->element_size);
        stack->top ++;
        return 1;
    }
}

int stack_pop(STACK stack, void *element)
{
    if (stack != NULL && stack->top != 0)
    {
        memcpy(element, stack->data + stack->top * stack->element_size - stack->element_size, stack->element_size);
        stack->top --;
        return 1;
    }
    return 0;
}

void stack_destroy(STACK stack)
{
    free(stack->data);
    free(stack);
}

void *stack_top(STACK stack)
{
    if (stack == NULL)
    {
        return NULL;
    }
    return stack->data + stack->top * stack->element_size - stack->element_size;
}

int stack_gettop(STACK stack, void *element)
{
    if (stack == NULL)
    {
        return 0;
    }
    memcpy(element, stack->data + stack->top * stack->element_size - stack->element_size, stack->element_size);
    return 1;
}
