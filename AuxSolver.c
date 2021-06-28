#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Solver.h"
#include "SetGameMap.h"
#include "utilities.h"
#include "Stack.h"
#include "AuxSolver.h"

// ******************************************************************************
// * Tent_Valid()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              Xi - coordenada da célula do mapa a avaliar
// * Returns:     1 se é possível colocar uma tenda na célula do mapa em avaliação
// *              -1 se não é possível colocar uma tenda na célula do mapa em avaliação
// * Description: Esta função recebe uma célula do mapa e verifica se esta é uma candidata viável
// *              a receber uma tenda.
// *              Para que a célula seja dada como válida tem que cumprir as seguintes condições:
// *                - ser um ponto
// *                - o numero máximo de tendas na linha ainda permitir que se coloque mais uma tenda
// *                - o numero máximo de tendas na coluna ainda permitir que se coloque mais uma tenda
// *                - não ter tendas em nenhuma das 8 células adjacentes
// *****************************************************************************
int Tent_Valid(MapInfo *GameMap, int Xi){
  int tL=0, tC=0, Li=0, Ci=0, i=0;

  Li=get_line_coordinate(GameMap, Xi);
  Ci=get_column_coordinate(GameMap, Xi);
  //retorna imediatamente erro se a célula não for um ponto
  if(Map_get_matrix(GameMap, Xi)!='.') return 0;
  //retorna imediatamente erro se targets esgotados
  for(i=get_coordinates(GameMap,0,Ci); i<get_coordinates(GameMap,Map_get_lines(GameMap),Ci); i+=Map_get_columns(GameMap)){
    if(Map_get_matrix(GameMap, i)=='T') tC++;
  }
  for (i=get_coordinates(GameMap, Li, 0); i<get_coordinates(GameMap, Li, Map_get_columns(GameMap)); i++){
    if(Map_get_matrix(GameMap, i)=='T') tL++;
  }
  if(tC>=Map_get_TentsColumn(GameMap, Ci) || tL>=Map_get_TentsLine(GameMap, Li)) return 0;
  //retorna imediatamente erro se existirem tendas adjacentes
  if(Li!=0){
    if(Map_get_matrix(GameMap, get_coordinates(GameMap, Li-1, Ci))=='T') return 0;
    if(Ci!=(Map_get_columns(GameMap)-1)){
      if (Map_get_matrix(GameMap, get_coordinates(GameMap, Li-1, Ci+1))=='T') return 0;
    }
    if(Ci!=0){
      if (Map_get_matrix(GameMap, get_coordinates(GameMap, Li-1, Ci-1))=='T') return 0;
    }
  }
  if(Ci!=0){
    if(Map_get_matrix(GameMap, get_coordinates(GameMap, Li, Ci-1))=='T') return 0;
  }
  if(Ci!=(Map_get_columns(GameMap)-1)){
    if(Map_get_matrix(GameMap, get_coordinates(GameMap, Li, Ci+1))=='T') return 0;
  }
  if(Li!=(Map_get_lines(GameMap)-1)){
    if(Map_get_matrix(GameMap, get_coordinates(GameMap, Li+1, Ci))=='T') return 0;
    if(Ci!=0){
      if (Map_get_matrix(GameMap, get_coordinates(GameMap, Li+1, Ci-1))=='T') return 0;
    }
    if(Ci!=(Map_get_columns(GameMap)-1)){
      if(Map_get_matrix(GameMap, get_coordinates(GameMap, Li+1, Ci+1))=='T') return 0;
    }
  }
  //retorna sucesso se não encontrou problemas
  return 1;
}

// ******************************************************************************
// * put_grass_around_tent_decision()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              Xi - coordenada da célula do mapa a avaliar (tenda colocada)
// *              InfoStack - estrutura que guarda as alterações feitas ao mapa durante a sua resolução
// * Returns:     void
// * Description: Esta função recebe a célula do mapa onde se acabou de colocar uma tenda
// *              e coloca relva nas células adjacentes que tenham pontos, uma vez que estas
// *              deixam de ser candidatas para a colocação de tendas.
// *              Esta é uma medida de otimização do tempo de resolução do problema
// *              uma vez que restringe as células candidatas a receber tendas.
// *              Cada vez que coloca uma relva carrega essa informação na InfoStack (pilha)
// *              para registar a alteração ao mapa e permitir que, caso o processo recursivo venha a decidir
// *              retirar a tenda de Xi, as relvas que esta função colocou possam também ser retiradas
// *              para recolocar o mapa na mesma situação que estava antes da colocação da tenda em Xi.
// *****************************************************************************
void put_grass_around_tent_decision(MapInfo* GameMap, int Xi, StackInfo *InfoStack){

  int Li=0, Ci=0;

  Li=get_line_coordinate(GameMap, Xi);
  Ci=get_column_coordinate(GameMap, Xi);
  if(Li!=0){
    if(Map_get_matrix(GameMap, Xi-Map_get_columns(GameMap))=='.'){
      Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), 'R');
      Push(InfoStack, Xi-Map_get_columns(GameMap), 'C');
    }
    if(Ci!=(Map_get_columns(GameMap)-1)){
      if (Map_get_matrix(GameMap, Xi-Map_get_columns(GameMap)+1)=='.') {
        Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap)+1, 'R');
        Push(InfoStack, Xi-Map_get_columns(GameMap)+1, 'C');
      }
    }
    if(Ci!=0){
      if (Map_get_matrix(GameMap, Xi-Map_get_columns(GameMap)-1)=='.'){
        Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap)-1, 'R');
        Push(InfoStack, Xi-Map_get_columns(GameMap)-1, 'C');
      }
    }
  }
  if(Ci!=0){
    if(Map_get_matrix(GameMap, Xi-1)=='.'){
      Map_write_matrix(GameMap, Xi-1, 'R');
      Push(InfoStack, Xi-1, 'C');
    }
  }
  if(Ci!=(Map_get_columns(GameMap)-1)){
    if(Map_get_matrix(GameMap, Xi+1)=='.') {
      Map_write_matrix(GameMap, Xi+1, 'R');
      Push(InfoStack, Xi+1, 'C');
    }
  }
  if(Li!=(Map_get_lines(GameMap)-1)){
    if(Map_get_matrix(GameMap, Xi+Map_get_columns(GameMap))=='.'){
      Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), 'R');
      Push(InfoStack, Xi+Map_get_columns(GameMap), 'C');
    }
    if(Ci!=0){
      if (Map_get_matrix(GameMap, Xi+Map_get_columns(GameMap)-1)=='.') {
        Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap)-1, 'R');
        Push(InfoStack, Xi+Map_get_columns(GameMap)-1, 'C');
      }
    }
    if(Ci!=(Map_get_columns(GameMap)-1)){
      if(Map_get_matrix(GameMap, Xi+Map_get_columns(GameMap)+1)=='.'){
        Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap)+1, 'R');
        Push(InfoStack, Xi+Map_get_columns(GameMap)+1, 'C');
      }
    }
  }
}

// ******************************************************************************
// * put_grass_around_tree_decision()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              Xi - coordenada da célula do mapa a avaliar
// * Returns:     retorna o número de relvas colocadas
// * Description: Esta função recebe uma célula do mapa que tem uma árvore à volta da qual não foi possível
// *              colocar uma tenda e coloca relva nas quatro células à sua volta que contenham pontos,
// *              precisamente as células que foram testadas, sem sucesso, para receber uma tenda.
// *              Esta é uma medida de otimização do tempo de resolução do problema
// *              uma vez que restringe as células candidatas a receber tendas.
// *              Cada vez que coloca uma relva carrega essa informação na InfoStack
// *              para registar a alteração ao mapa e permitir que, caso o processo recursivo venha a requerer
// *              que se volte a reavaliar esta árvore se possa recolocar o mapa na situação anterior.
// *****************************************************************************
int put_grass_around_tree_decision(MapInfo* GameMap, int Xi, StackInfo *InfoStack){
  int Li=0, Ci=0, grass=0;

  Li=get_line_coordinate(GameMap, Xi);
  Ci=get_column_coordinate(GameMap, Xi);
  if(Li!=0){
    if(Map_get_matrix(GameMap, Xi-Map_get_columns(GameMap))=='.'){
      Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), 'R');
      Push(InfoStack, Xi-Map_get_columns(GameMap), 'C');
      grass++;
    }
  }
  if(Ci!=0){
    if(Map_get_matrix(GameMap, Xi-1)=='.'){
      Map_write_matrix(GameMap, Xi-1, 'R');
      Push(InfoStack, Xi-1, 'C');
      grass++;
    }
  }
  if(Ci!=(Map_get_columns(GameMap)-1)){
    if(Map_get_matrix(GameMap, Xi+1)=='.') {
      Map_write_matrix(GameMap, Xi+1, 'R');
      Push(InfoStack, Xi+1, 'C');
      grass++;
    }
  }
  if(Li!=(Map_get_lines(GameMap)-1)){
    if(Map_get_matrix(GameMap, Xi+Map_get_columns(GameMap))=='.'){
      Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), 'R');
      Push(InfoStack, Xi+Map_get_columns(GameMap), 'C');
      grass++;
    }
  }
  return grass;
}

// ******************************************************************************
// * Create_Stack()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              trees - vetor que guarda as posições das árvores a analisar
// * Returns:     InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução (define a pilha)
// * Description: Esta função cria e aloca o vetor com as posições das àrvores e cria e define a pilha ao obter o seu
//                tamanho (numero de pontos no mapa)
// *****************************************************************************
StackInfo* Create_Stack (MapInfo *GameMap, int* trees[]){
  int i=0, j=0;
  int stack_size=0;
  StackInfo *InfoStack;
  *trees =(int*)check_calloc(sizeof(int), Map_get_NbrTrees(GameMap));
  for (i=0; i<Map_get_lines(GameMap)*Map_get_columns(GameMap); i++){
    if(Map_get_matrix(GameMap, i)=='.') stack_size++;
    if(Map_get_matrix(GameMap, i)=='A') {
      (*trees)[j]=i;
      j++;
    }
  }
  InfoStack=Init(stack_size);
  return InfoStack;
}

// ******************************************************************************
// * Print_Results_Error()
// * Arguments:   Results- ficheiro de saída
//                GameMap - estrutura com a informação do mapa em análise
// *              erro - se o mapa é valido ou inválido (virá sempre a -1)
// * Returns:     void
// * Description: Esta função escreve erro no ficheiro de saída, ou seja,
//                escreve os valores das linhas, colunas e -1
// *****************************************************************************
void Print_Results_Error(FILE *Results, MapInfo* GameMap, int erro){
  fprintf(Results, "%hd %hd %d\n\n", Map_get_lines(GameMap), Map_get_columns(GameMap), erro);
}

// ******************************************************************************
// * Print_Results()
// * Arguments:   Results- ficheiro de saída
//                GameMap - estrutura com a informação do mapa em análise
// *              erro - se o mapa é valido ou inválido (virá sempre a 1)
// * Returns:     void
// * Description: Esta função escreve sucesso no ficheiro de saída, ou seja,
//                escreve os valores das linhas, colunas, 1 e o mapa final resolvido
// *****************************************************************************
void Print_Results(FILE *Results, MapInfo* GameMap, int erro){
  int j=0, i=0;
  fprintf(Results, "%hd %hd %d\n", Map_get_lines(GameMap), Map_get_columns(GameMap), erro);
  for(j=0; j<Map_get_lines(GameMap); j++){
    for(i=0; i<Map_get_columns(GameMap); i++){
      if(Map_get_matrix(GameMap, get_coordinates(GameMap, j, i))=='a') Map_write_matrix(GameMap, get_coordinates(GameMap, j, i), 'A');
      if(Map_get_matrix(GameMap, get_coordinates(GameMap, j, i))=='R') Map_write_matrix(GameMap, get_coordinates(GameMap, j, i), '.');
      fprintf(Results, "%c", Map_get_matrix(GameMap, get_coordinates(GameMap, j, i)));
    }
    fprintf(Results, "\n");
  }
}
