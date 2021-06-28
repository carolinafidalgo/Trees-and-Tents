#include <stdio.h>
#include <stdlib.h>
#include "SetGameMap.h"
#include "utilities.h"
#include "Solver.h"
#include "AuxSolver.h"

int main(int argc, char* argv[]){

  FILE *Maps = NULL, *Results = NULL;
  FileInfo *Info = NULL;
  MapInfo *GameMap = NULL;
  char *find=NULL;
  int erro=1;
  int *trees=NULL, position=0, PlacedTents=0;
  StackInfo *InfoStack=NULL;

  if(argc != 2) exit(0);
  check_arguments(argv); //confirmar se os argumentos são válidos
  Maps=fopen(argv[1], "r"); //abertura do ficheiro de entrada para leitura
  if(Maps == NULL) exit(0);
  find=(char*)check_calloc(sizeof(char), strlen(argv[1])-5+strlen(".tents")+1);
  strncpy(find, argv[1], strlen(argv[1])-5);
  strcat(find, ".tents");
  Results=fopen(find, "w"); //abertura do ficheiro de saída para escrita
  if(Results == NULL) exit(0);
  free(find);
  //primeira leitura do ficheiro para encontrar valores max para alocacao
  Info=readFile(Maps);
  rewind(Maps);
  //alocacao da estrutura pelos valores maximos encontrados
  GameMap=MemoryAllocation(Info);
  while(1){  //ciclo de tratamento de todos os mapas do ficheiro
    erro=1;
    if(feof(Maps)) break; //sai do ciclo quando encontra o end of file
    erro=readMap(Maps, GameMap, Info);
    if(erro==-1) Print_Results_Error(Results, GameMap, erro); //mapa não valido
    else if(erro==0) Print_Results(Results, GameMap, 1); //mapa valido já solucionado por não ter tendas
    else { //mapa válido para solucionar
      InfoStack=Create_Stack(GameMap, &trees);
      if(Map_get_Season(GameMap)=='H') erro=Solve_MapH(GameMap, position, trees, InfoStack);
      else if(Map_get_Season(GameMap)=='L') erro=Solve_MapL(GameMap, position, trees, InfoStack, PlacedTents);
      if(erro==1) Print_Results(Results, GameMap, erro); //mapa com solução encontrada
      else if(erro==-1) Print_Results_Error(Results, GameMap, erro); //mapa sem solução encontrada
      free(trees); //free do vetor das arvores
      Empty(InfoStack); //free da pilha
    }
    if(fscanf(Maps,"\n")!=0) exit(0);
  }
  fclose(Maps); //fechar ficheiro de leitura
  fclose(Results); //fechar ficheiro de escrita
  Free_Struct1(GameMap); //free da estrutura que guarda a caracterização do mapa
  Free_Struct2(Info); //free da estrutura que guarda os valores da alocação
  return 0;
}
