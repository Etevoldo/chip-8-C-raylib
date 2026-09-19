#ifndef STACK_H
#define STACK_H
#include "types.h"

// standard stack data structure
void stackPush(Stack *stack, u16 value);
u16 stackPop(Stack *stack);

#endif