#ifndef AUXSOLVER_H
#define AUXSOLVER_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utilities.h"
#include "Stack.h"


void Print_Results_Error(FILE *Results, MapInfo* GameMap, int erro);
void Print_Results(FILE *Results, MapInfo* GameMap, int erro);
int put_grass_around_tree_decision(MapInfo* GameMap, int Xi, StackInfo *InfoStack);
void put_grass_around_tent(MapInfo* GameMap, int Xi);
void put_grass_around_tent_decision(MapInfo* GameMap, int Xi, StackInfo *InfoStack);
StackInfo* Create_Stack (MapInfo *GameMap, int **trees);
int Tent_Valid(MapInfo *GameMap, int Xi);

#endif
