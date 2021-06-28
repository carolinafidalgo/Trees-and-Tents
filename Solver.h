#ifndef SOLVER_H
#define SOLVER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SetGameMap.h"
#include "utilities.h"
#include "AuxSolver.h"
#include "Stack.h"


int Mandatory_Tents_and_Grass (MapInfo *GameMap);
int Solve_MapH(MapInfo* GameMap, int position, int *trees, StackInfo *InfoStack);
int Solve_MapL(MapInfo* GameMap, int position, int *trees, StackInfo *InfoStack, int PlacedTents);

#endif
