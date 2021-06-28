#ifndef STACK_H
#define STACK_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utilities.h"

typedef struct Stack_Struct StackStruct;
typedef struct Stack_Info StackInfo;

StackInfo* Init(int stack_size);
StackInfo* Push(StackInfo *InfoStack, int Xi, char type);
void Pop(StackInfo *InfoStack);
void Empty(StackInfo *InfoStack);
int Stack_get_Xi(StackInfo* InfoStack, int i);
char Stack_get_type(StackInfo* InfoStack, int i);
int Stack_get_index(StackInfo* InfoStack);
int Stack_get_size(StackInfo* InfoStack);

#endif
