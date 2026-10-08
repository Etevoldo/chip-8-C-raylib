#pragma once
#include <stdbool.h>

typedef struct {
    int length;
    int capacity;
    int *arr;
} List;

List list_create(int size);
bool list_contains(List *list, int value);
void list_push(List *list, int value);
void list_remove(List *list, int index);
void list_print(List *list);
void list_dealloc(List *list);