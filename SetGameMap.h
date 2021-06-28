#ifndef SETGAMEMAP_H
#define SETGAMEMAP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

typedef struct Map_Info MapInfo;
typedef struct File_Info FileInfo;

FileInfo* readFile (FILE* Maps);
MapInfo* MemoryAllocation(FileInfo *Info);
int readMap (FILE* Maps, MapInfo *GameMap, FileInfo *Info);
int get_coordinates(MapInfo* GameMap, int L0, int C0);
int get_line_coordinate(MapInfo* GameMap, int Xi);
int get_column_coordinate(MapInfo* GameMap, int Xi);
int Map_get_lines(MapInfo* GameMap);
int Map_get_columns(MapInfo* GameMap);
int Map_get_TentsLine(MapInfo* GameMap, int i);
int Map_get_TentsColumn(MapInfo* GameMap, int i);
int Map_get_NbrTrees(MapInfo* GameMap);
int Map_get_NbrTents(MapInfo* GameMap);
char Map_get_matrix(MapInfo* GameMap, int Xi);
void Map_write_matrix(MapInfo* GameMap, int Xi, char dot);
void Free_Struct1(MapInfo* GameMap);
void Free_Struct2(FileInfo* Info);
char Map_get_Season(MapInfo* GameMap);

#endif
