#ifndef STACK_H
#define STACK_H
#include "types.h"

typedef struct {
    u8  index;
    u16 arr[16];
} Stack;

// standard stack data structure
void stackPush(Stack *, u16);
u16 stackPop(Stack *);

#endif