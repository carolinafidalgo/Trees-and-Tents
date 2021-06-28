#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "utilities.h"
#include "SetGameMap.h"
#include "Solver.h"
#include "AuxSolver.h"

//estrutura que guarda as informacoes dos mapas
struct Map_Info{
  int lines;
  int columns;
  int* TentsLine;
  int* TentsColumn;
  char* matrix;
  int NbrTrees;
  int NbrTents;
  char Season;
};

//estrutura que guarda valores maximos para alocacao
struct File_Info{
  int MMax;
  int VLMax;
  int VCMax;
};

// ******************************************************************************
// * readFile()
// * Arguments:   Maps - ficheiro de mapas passado como argumento ao programa
// * Returns:     apontador para estrutura que contem os valores do maior mapa valido
// * Description: Para otimizar a utilização da memória, esta função corre todo o ficheiro
// *              para detetar o maior mapa válido, evitando assim a necessidade de alocar muita
// *              memória para posteriomente armazenar um mapa muito grande ainda que inválido.
// *              Validações feitas ao mapa para o aceitar como válido:
// *                - somatório de tendas do vetor linhas igual ao somatorio de tendas do vetor colunas
// *                - numero de árvores no mapa maior que numero de tendas a colocar
// *                - numero de pontos no mapa maior que numero de tendas a colocar
// *                - numero de tendas a colocar em cada linha/coluna não pode ser negativo
// *                - numero de tendas a colocar em cada linha/coluna não pode ser superior a metade
// *                  do numero de colunas/linhas porque as tendas não se podem tocar
// *****************************************************************************
FileInfo* readFile (FILE* Maps){

  FileInfo *Info;
  char *auxstr=NULL;
  int auxint=0, nbrtrees=0, nbrdots=0;
  int l=0, c=0, m=0, i=0, j=0;
  int SumLines=0, SumColumns=0, maxtentsL=0, maxtentsC=0, erro=1;

  Info=(FileInfo *)check_calloc(sizeof(FileInfo), 1);
  Info->VLMax=0; //valor maximo para o vetor das linhas
  Info->VCMax=0; //valor maximo para o vetor das colunas
  Info->MMax=0; //dim max da matriz

  while (1) {   //ciclo de leitura de todos os mapas do ficheiro
    nbrtrees=0;
    nbrdots=0;
    SumLines=0;
    SumColumns=0;
    auxint=0;
    if(feof(Maps)!=0) break; //sai do ciclo quando encontra o end of file
    if(fscanf(Maps, "%d %d", &l, &c)!=2) exit(0);
    maxtentsL=round(((float)c+0.5)/2); //variavel para limitar o numero de tendas por linha
    maxtentsC=round(((float)l+0.5)/2); //variavel para limitar o numero de tendas por coluna
    for(i=0; i<l; i++){ //ler vetor linhas para contar total de tendas a colocar e validar limites de tendas por linha
      if(fscanf(Maps, "%d", &auxint)!=1) exit(0);
      SumLines+=auxint;
      if(auxint>maxtentsL || auxint<0) erro=-1;
    }
    for(i=0; i<c; i++){ //ler vetor colunas para contar total de tendas a colocar e validar limites de tendas por coluna
      if(fscanf(Maps, "%d", &auxint)!=1) exit(0);
      SumColumns+=auxint;
      if(auxint>maxtentsC || auxint<0) erro=-1;
    }
    m=l*c; //numero de celulas da matriz
    auxstr=(char*)check_calloc(sizeof(char), c+1);
    for(i=0; i<l; i++){ //ler matriz para contar numero de arvores e pontos
      if(fscanf(Maps, "%s", auxstr)!=1) exit(0);
      for(j=0; j<c; j++){
        if(auxstr[j]=='A') nbrtrees++;
        else nbrdots++;
      }
    }
    free(auxstr);
    //caso o mapa seja valida a partida guarda os valores para alocar só a maior do ficheiro
    if(SumLines==SumColumns && nbrtrees>=SumLines && nbrdots>=SumLines && erro!=-1){
      if(l>Info->VLMax) Info->VLMax=l;
      if(c>Info->VCMax) Info->VCMax=c;
      if(m>Info->MMax) Info->MMax=m;
    }
    if(fscanf(Maps,"\n")!=0) exit(0);

  }
  return(Info);
}

// ******************************************************************************
// * MemoryAllocation()
// * Arguments:   Info - estrutura que contem os valores do maior mapa vaildo
// * Returns:     apontador para estrutura que vai guardar a informação dos mapas à medida que vao sendo lidos
// * Description: A funnção aloca a estrutura que vai guardar a informação dos mapas a analisar
// *              pelos valores que correspondem ao maior mapa valido do ficheiro de mapas.
// *****************************************************************************
MapInfo* MemoryAllocation(FileInfo *Info){
  MapInfo* GameMap;
  GameMap=(MapInfo*)check_calloc(sizeof(MapInfo),1);
  GameMap->TentsLine=(int*)check_calloc(sizeof(int), Info->VLMax);
  GameMap->TentsColumn=(int*)check_calloc(sizeof(int), Info->VCMax);
  if(Info->MMax!=0) GameMap->matrix=(char*)check_calloc(sizeof(char), (Info->MMax+1));
  return(GameMap);
}

// ******************************************************************************
// * readMap()
// * Arguments:   Maps - ficheiro de mapas passado como argumento ao programa
// *              GameMap - estrutura que vai guardar a informação dos mapas à medida que vao sendo lidos
// *              Info - estrutura que contem os valores do maior mapa valido
// * Returns:     1 se leu para GameMap um mapa válido
// *              -1 se leu para GameMap um mapa inválido
// *              0 se leu para GameMap um mapa válido e sem tendas a colocar
// * Description: A função lê o mapa para a estrutura GameMap assegurando quer a confirmação da sua validade
// *              quer a sua otimização para acelerar o seu processo de resolução.
// *              Validações feitas ao mapa para o aceitar como válido:
// *                - o numero de linhas e colunas não é maior que o numero de linhas e colunas do maior mapa válido
// *                - somatório de tendas do vetor linhas igual ao somatorio de tendas do vetor colunas
// *                - o numero de celulas da matriz não é maior que o numero celulas da matriz do maior mapa válido
// *                - numero de tendas a colocar em cada linha/coluna não pode ser negativo
// *                - numero de árvores no mapa maior que numero de tendas a colocar
// *               Otimizações feitas ao mapa para acelerar a sua resolução:
// *                - se a linha ou coluna de uma célula com ponto tem o target a 0, poe relva na célula
// *                - se uma célula com ponto não tem árvores à volta, coloca relva na célula
// *                - se uma célula com árvore nao tem pontos à volta "anulamos" essa arvores do mapa
// *****************************************************************************
int readMap (FILE* Maps, MapInfo *GameMap, FileInfo *Info){
  int i=0, j=0, auxint=0;
  int SumLines=0, SumColumns=0;
  int flag=0;
  char *auxstr=NULL;
  int tree=0, dot=0;
  int Xi=0;
  int maxtentsC=0, maxtentsL=0;

  GameMap->NbrTrees=0;
  GameMap->NbrTents=0;
  GameMap->lines=0;
  GameMap->columns=0;

  if(fscanf(Maps, "%d %d", &GameMap->lines, &GameMap->columns)!=2) exit(0);

  maxtentsL=round(((float)GameMap->columns+0.5)/2);
  maxtentsC=round(((float)GameMap->lines+0.5)/2);

  if(GameMap->lines>Info->VLMax || GameMap->columns>Info->VCMax){
    //Se mapa que tem mais linhas ou colunas que o maior mapa válido
    //passa o resto do mapa sem o ler para a estrutura GameMap e sai da função retornando erro
    for (i=0; i<GameMap->lines; i++){ //vetor linhas
      if(fscanf(Maps, "%d", &auxint)!=1) exit(0);
    }
    for (i=0; i<GameMap->columns; i++){ //vetor colunas
      if(fscanf(Maps, "%d", &auxint)!=1) exit(0);
    }
    auxstr=(char*)check_calloc(sizeof(char),GameMap->columns+1);
    for (i=0; i<GameMap->lines; i++){ //vetor mapa
      if(fscanf(Maps, "%s", auxstr)!=1) exit(0);
    }
    free(auxstr);
    return -1;
  }
  for (i=0; i<GameMap->lines; i++){ //ler vetor linhas para GameMap e validar que o numero de tendas não é negativo
    if(fscanf(Maps, "%d", &GameMap->TentsLine[i])!=1) exit(0);
    SumLines+=GameMap->TentsLine[i];
    if(GameMap->TentsLine[i]>maxtentsL || GameMap->TentsLine[i]<0) flag=1; //não pode ter valores negativos
  }
  for (i=0; i<GameMap->columns; i++){ //ler vetor colunas para GameMap e validar que o numero de tendas não é negativo
    if(fscanf(Maps, "%d", &GameMap->TentsColumn[i])!=1) exit(0);
    SumColumns+=GameMap->TentsColumn[i];
    if(GameMap->TentsColumn[i]>maxtentsC || GameMap->TentsColumn[i]<0) flag=1; //não pode ter valores negativos
  }
  if(SumLines!=SumColumns || (GameMap->lines*GameMap->columns)>Info->MMax || flag==1){
    //Se mapa que somas diferentes de numero de tendas em linhas e colunas ou
    //tem numero de celulas superior ao maior mapa válido ou se tem numeros de árvores negativos
    //passa o resto do mapa sem o ler para a estrutura GameMap e sai da função retornando erro
    auxstr=(char*)check_calloc(sizeof(char),GameMap->columns+1);
    for (i=0; i<GameMap->lines; i++){ //vetor mapa
      if(fscanf(Maps, "%s", auxstr)!=1) exit(0);
    }
    free(auxstr);
    return -1;
  }
  auxstr=(char*)check_calloc(sizeof(char),GameMap->columns+1);
  for (i=0; i<GameMap->lines; i++){ //ler vetor mapa para GameMap
    if(fscanf(Maps, "%s", auxstr)!=1) exit(0);
    for(j=0; j<GameMap->columns; j++){
      GameMap->matrix[j+i*GameMap->columns]=auxstr[j];
    }
  }
  free(auxstr);

  if(SumLines==0 && SumColumns==0) return 0; //caso nao haja tendas o mapa está correto à partida

  //otimizar o mapa colocando relva
  for (i=0; i<GameMap->lines; i++) {
    for(j=0; j<GameMap->columns; j++){
      Xi=i*GameMap->columns+j;
      tree=0;
      if(GameMap->matrix[Xi]=='.'){ // procura células com pontos que possam ser transformados em relva
        //se linha ou coluna da célula tem o target a 0, poe relva na célula
        if(GameMap->TentsLine[i]==0 || GameMap->TentsColumn[j]==0) GameMap->matrix[Xi]='R';
        if(i!=0){
          if((GameMap->matrix[Xi-GameMap->columns])=='A') tree++;
        }
        if(j!=GameMap->columns-1){
          if((GameMap->matrix[Xi+1])=='A') tree++;
        }
        if(i!=GameMap->lines-1){
          if((GameMap->matrix[Xi+GameMap->columns])=='A') tree++;
        }
        if(j!=0){
          if((GameMap->matrix[Xi-1])=='A') tree++;
        }
        if(tree==0) GameMap->matrix[Xi]='R'; //se célula não tem árvores à volta, coloca relva na célula

      }
    }
  }

  //otimizar o mapa "apagando" árvores
  for (i=0; i<GameMap->lines; i++) {
    for(j=0; j<GameMap->columns; j++){
      Xi=i*GameMap->columns+j;
      dot=0;
      if(GameMap->matrix[Xi]=='A'){ //procura células com árvores que possam ser "eliminadas do mapa"
      if(i!=0){
        if((GameMap->matrix[Xi-GameMap->columns])=='.') dot++;
      }
      if(j!=GameMap->columns-1){
        if((GameMap->matrix[Xi+1])=='.') dot++;
      }
      if(i!=GameMap->lines-1){
        if((GameMap->matrix[Xi+GameMap->columns])=='.') dot++;
      }
      if(j!=0){
        if((GameMap->matrix[Xi-1])=='.') dot++;
      }
      if(dot==0) GameMap->matrix[Xi]='a'; //se uma árvore nao tem pontos à volta "anulamos" essa arvore
      else GameMap->NbrTrees++;
    }
  }
}

GameMap->NbrTents=SumLines;
if(GameMap->NbrTrees<GameMap->NbrTents) return -1; //se mapa tem menos árvores que tendas a colocar sai da função retornando erro
//if(flag==1) return -1; //se mapa viola capacidades das linhas ou colunas sai da função retornando erro
if(GameMap->NbrTrees>GameMap->NbrTents) GameMap->Season='L';
else GameMap->Season='H';
return 1;
}

//passa de coordenadas duplas para uma só (vetor)
int get_coordinates(MapInfo* GameMap, int L0, int C0){
  int Xi;
  Xi=L0*GameMap->columns+C0;
  return(Xi);
}

//retirar o L da coordenada unitaria
int get_line_coordinate(MapInfo* GameMap, int Xi){
  int Li=0;
  Li=floor(Xi/GameMap->columns);
  return (Li);
}

//retirar o C da coordenada unitaria
int get_column_coordinate(MapInfo* GameMap, int Xi){
  int Ci=0;
  Ci=Xi%GameMap->columns;
  return (Ci);
}

int Map_get_lines(MapInfo* GameMap) {
  return GameMap->lines;
}

int Map_get_columns(MapInfo* GameMap) {
  return GameMap->columns;
}

int Map_get_TentsLine(MapInfo* GameMap, int i) {
  return GameMap->TentsLine[i];
}

int Map_get_TentsColumn(MapInfo* GameMap, int i) {
  return GameMap->TentsColumn[i];
}

char Map_get_Season(MapInfo* GameMap) {
  return GameMap->Season;
}

int Map_get_NbrTrees(MapInfo* GameMap) {
  return GameMap->NbrTrees;
}

int Map_get_NbrTents(MapInfo* GameMap) {
  return GameMap->NbrTents;
}

char Map_get_matrix(MapInfo* GameMap, int Xi) {
  return GameMap->matrix[Xi];
}

void Map_write_matrix(MapInfo* GameMap, int Xi, char dot){
  GameMap->matrix[Xi]=dot;
}

void Free_Struct1(MapInfo* GameMap){
  free(GameMap->TentsLine);
  free(GameMap->TentsColumn);
  free(GameMap->matrix);
  free(GameMap);
}

void Free_Struct2(FileInfo* Info){
  free(Info);
}
