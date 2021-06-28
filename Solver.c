#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Solver.h"
#include "SetGameMap.h"
#include "utilities.h"
#include "Stack.h"
#include "AuxSolver.h"

// ******************************************************************************
// * Solve_MapH()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              position - indice da árvore a analisar
// *              trees - vetor com as árvores que podem receber uma tenda
// *              InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Returns:     1 se conseguiu por uma tenda à volta da árvore
// *              0 se não conseguiu por uma tenda à volta da árvore
// * Description: Esta é uma função recursiva utilizada para resolver uma mapa de época alta, ou seja
// *              um mapa em que o número de árvores disponíveis para receber uma tenda é igual ao
// *              número de tendas a colocar.
// *              Por ser recursiva, o seu funcionamento é o seguinte:
// *                - é chamada a primeira vez a partir do main() com position=0, ou seja, para colocar
// *                  uma tenda na primeira árvore disponível, a árvore de indice 0 do vetor trees
// *                - coloca a tenda tentativamente na primeira célula possível das quatro células à volta da tenda
// *                  e chama-se a si mesma para colocar tenda na próxima árvore do vetor trees
// *                - em cada chamada faz exatamente o mesmo e cada vez que consegue colocar uma tenda
// *                  segue para a próxima árvore do vetor trees chamando-se novamente a si mesma
// *                  com incremento da variável position, avançando assim numa solução tentativa
// *                - se em algum momento não conseguir colocar uma tenda, significa que a solução que,
// *                  tentativamente, se estava a constriur, não é válida pelo que tem que retornar erro e voltar
// *                  à árvore anterior para explorar outra das quatro células possíveis e seguir em frente
// *                  na procura de uma soluçao
// *              As condições de paragem, ou seja, as condições que fazem retornar a recursividade
// *              ao nível anterior, são as seguintes:
// *                - se a position for igual ao número de árvores disponíveis, então significa que
// *                  conseguimos colocar todas as tendas, ou seja, a solução que estivemos a construir
// *                  tentativamente é a solução válida do problema e, nesse caso, retornamos sucesso em cascata
// *                  até à função main()
// *                - se nenhuma das quatro células à volta de uma árvore permite criar uma solução válida,
// *                  então retornamos insucesso para o nível de recursividade anterior que tentará
// *                  colocar a tenda noutra célula da árvore anterior e continuar a procurar uma solução válida
// *                - se chegarmos à situação em que main() recebe informação de insucesso, isso significa
// *                  que todas as alternativas foram testadas e que se produziu um retorno de insucesso em cascata
// *                  até ao main()
// *****************************************************************************
int Solve_MapH(MapInfo* GameMap, int position, int *trees, StackInfo *InfoStack){
  int flag=0, Xi=0, erro=0, Li=0, Ci=0;

  //atingimos o fim do vetor trees com sucesso pelo que produzimos uma solução válida
  if (position==Map_get_NbrTrees(GameMap)) return 1;

  Xi=trees[position];
  Li=get_line_coordinate(GameMap, Xi);
  Ci=get_column_coordinate(GameMap, Xi);

  //colocar tenda em cima
  flag=0;
  if (Li!=0) {
    flag=Tent_Valid(GameMap, Xi-Map_get_columns(GameMap));
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), 'T');
    Push(InfoStack, Xi-Map_get_columns(GameMap), 'D');
    put_grass_around_tent_decision(GameMap, Xi-Map_get_columns(GameMap), InfoStack);
    erro=Solve_MapH(GameMap, position+1, trees, InfoStack);
    //recebemos sucesso de todas os níveis de recursividade seguintes
    if(erro==1) return 1;
    //recebemos insucesso dos níveis de recursividade seguintes pelo que temos que apagar a tenda
    //e relvas que colocamos para repor o mapa e de seguida tentar colocar a tenda
    //noutra celula à volta desta árvore para tentar produzir uma solução válida
    if(erro==-1){
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), '.');
      Pop(InfoStack);
    }
  }
  //colocar tenda à direita
  flag=0;
  if (Ci!=(Map_get_columns(GameMap)-1)) {
    flag=Tent_Valid(GameMap, Xi+1);
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi+1, 'T');
    Push(InfoStack, Xi+1, 'D');
    put_grass_around_tent_decision(GameMap, Xi+1, InfoStack);
    erro=Solve_MapH(GameMap, position+1, trees, InfoStack);
    //recebemos sucesso de todas os níveis de recursividade seguintes
    if(erro==1) return 1;
    //recebemos insucesso dos níveis de recursividade seguintes pelo que temos que apagar a tenda
    //e relvas que colocamos para repor o mapa e de seguida tentar colocar a tenda
    //noutra celula à volta desta árvore para tentar produzir uma solução válida
    if(erro==-1){
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      Map_write_matrix(GameMap, Xi+1, '.');
      Pop(InfoStack);
    }
  }
  //colocar tenda em baixo
  flag=0;
  if (Li!=(Map_get_lines(GameMap)-1)) {
    flag=Tent_Valid(GameMap, Xi+Map_get_columns(GameMap));
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), 'T');
    Push(InfoStack, Xi+Map_get_columns(GameMap), 'D');
    put_grass_around_tent_decision(GameMap, Xi+Map_get_columns(GameMap), InfoStack);
    erro=Solve_MapH(GameMap, position+1, trees, InfoStack);
    //recebemos sucesso de todas os níveis de recursividade seguintes
    if(erro==1) return 1;
    //recebemos insucesso dos níveis de recursividade seguintes pelo que temos que apagar a tenda
    //e relvas que colocamos para repor o mapa e de seguida tentar colocar a tenda
    //noutra celula à volta desta árvore para tentar produzir uma solução válida
    if(erro==-1){
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), '.');
      Pop(InfoStack);
    }
  }
  //colocar tenda à esquerda
  flag=0;
  if (Ci!=0) {
    flag=Tent_Valid(GameMap, Xi-1);
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi-1, 'T');
    Push(InfoStack, Xi-1, 'D');
    put_grass_around_tent_decision(GameMap, Xi-1, InfoStack);
    erro=Solve_MapH(GameMap, position+1, trees, InfoStack);
    //recebemos sucesso de todas os níveis de recursividade seguintes
    if(erro==1) return 1;
    //recebemos insucesso dos níveis de recursividade seguintes pelo que temos que apagar a tenda
    //e relvas que colocamos para repor o mapa e de seguida tentar colocar a tenda
    //noutra celula à volta desta árvore para tentar produzir uma solução válida
    if(erro==-1){
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      Map_write_matrix(GameMap, Xi-1, '.');
      Pop(InfoStack);
    }
  }
  //recebemos insucesso dos níveis de recursividade seguintes e esgotamos todas as hipoteses
  //à volta desta tenda pelo que temos que retornar insucesso para o nível de recursividade anteriores
  //para que este tente encontrar nova solução possível
  return -1;
}

// ******************************************************************************
// * Solve_MapL()
// * Arguments:   GameMap - estrutura com a informação do mapa em análise
// *              position - indice da árvore a analisar
// *              trees - vetor com as árvores que podem receber uma tenda
// *              InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// *              PlacedTents - número de tendas já colocadas nos níveis anteriores da recursividade
// * Returns:     1 se conseguiu uma decisão para a árvore em avaliação que faz parte da solução final
// *              0 se não conseguiu uma decisão para a árvore em avaliação que faça parte da solução final
// * Description: Esta é uma função recursiva utilizada para resolver uma mapa de época baixa, ou seja
// *              um mapa em que o número de árvores disponíveis para receber uma tenda é superior ao
// *              número de tendas a colocar.
// *              Por ser recursiva, o seu funcionamento é o seguinte:
// *                - é chamada a primeira vez a partir do main() com position=0 e PlacedTents=0, ou seja,
// *                  para colocar uma tenda na primeira árvore disponível, a árvore de indice 0 do vetor trees
// *                - coloca a tenda tentativamente na primeira célula possível das quatro células à volta da tenda
// *                  e chama-se a si mesma para colocar tenda na próxima árvore do vetor trees
// *                - em cada chamada faz exatamente o mesmo e cada vez que consegue colocar uma tenda
// *                  segue para a próxima árvore do vetor trees chamando-se novamente a si mesma
// *                  com incremento da variável position e da variável PlacedTents, avançando assim numa
// *                  solução tentativa
// *                - se em algum momento não conseguir colocar uma tenda, continua a tentar encontrar solução
// *                  aceitando que essa árvore fica sem tenda, uma vez que estamos em época baixa, chamando-se
// *                  a si mesma para a próxima iteração incrementando a variável position mas não aumentando
// *                  a variável PlacedTents.
// *              As condições de paragem, ou seja, as condições que fazem retornar a recursividade
// *              ao nível anterior, são as seguintes:
// *                - se já colocamos todas as tendas necessárias, então a solução que estivemos a
// *                  construir tentativamente é uma solução válida do problema e, nesse caso,
// *                  retornamos sucesso em cascata até à função main()
// *                - se a position for igual ao número de árvores disponíveis, então significa que
// *                  terminaram as árvores sem conseguirmos produzir uma solução válida e,
// *                  nesse caso, retornamos insucesso para o nivel de recursividade anterior que tentará
// *                  colocar a tenda noutra célula da árvore anterior e continuar a procurar uma solução válida
// *                - se o numero de árvores ainda disponível não for suficiente para o nºumero de tendas
//*                   ainda não colocadas, então já não conseguiremos produzir uma solução válida e,
// *                  nesse caso, retornamos insucesso para o nivel de recursividade anterior que tentará
// *                  colocar a tenda noutra célula da árvore anterior e continuar a procurar uma solução válida
// *                - se chegarmos à situação em que main() recebe informação de insucesso, isso significa
// *                  que todas as alternativas foram testadas e que se produziu um retorno de insucesso em cascata
// *                  até ao main()
// *****************************************************************************
int Solve_MapL(MapInfo* GameMap, int position, int *trees, StackInfo *InfoStack, int PlacedTents){

  int flag=0, Xi=0, erro=0, Li=0, Ci=0;
  int i=0, grass=0;

  if (PlacedTents == Map_get_NbrTents(GameMap)) return 1;
  if (position==Map_get_NbrTrees(GameMap)) return -1;
  if ((Map_get_NbrTrees(GameMap)-position)<(Map_get_NbrTents(GameMap)-PlacedTents)) return -1;

  Xi=trees[position];
  Li=get_line_coordinate(GameMap, Xi);
  Ci=get_column_coordinate(GameMap, Xi);

  //colocar tenda em cima
  flag=0;
  if (Li!=0) {
    flag=Tent_Valid(GameMap, Xi-Map_get_columns(GameMap));
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), 'T');
    Push(InfoStack, Xi-Map_get_columns(GameMap), 'D');
    put_grass_around_tent_decision(GameMap, Xi-Map_get_columns(GameMap), InfoStack);
    erro=Solve_MapL(GameMap, position+1, trees, InfoStack, PlacedTents+1);
    if(erro==1) return erro;
    if(erro==-1){
      //APAGAR RELVAS
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      //APAGAR TENDA
      Map_write_matrix(GameMap, Xi-Map_get_columns(GameMap), '.');
      Pop(InfoStack);
    }
  }

  //colocar tenda em baixo
  flag=0;
  if (Li!=(Map_get_lines(GameMap)-1)) {
    flag=Tent_Valid(GameMap, Xi+Map_get_columns(GameMap));
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), 'T');
    Push(InfoStack, Xi+Map_get_columns(GameMap), 'D');
    put_grass_around_tent_decision(GameMap, Xi+Map_get_columns(GameMap), InfoStack);
    erro=Solve_MapL(GameMap, position+1, trees, InfoStack, PlacedTents+1);
    if(erro==1) return erro;
    if(erro==-1){
      //APAGAR RELVAS
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      //APAGAR TENDA
      Map_write_matrix(GameMap, Xi+Map_get_columns(GameMap), '.');
      Pop(InfoStack);
    }
  }

  //colocar tenda à direita
  flag=0;
  if (Ci!=(Map_get_columns(GameMap)-1)) {
    flag=Tent_Valid(GameMap, Xi+1);
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi+1, 'T');
    Push(InfoStack, Xi+1, 'D');
    put_grass_around_tent_decision(GameMap, Xi+1, InfoStack);
    erro=Solve_MapL(GameMap, position+1, trees, InfoStack, PlacedTents+1);
    if(erro==1) return erro;
    if(erro==-1){
      //APAGAR RELVAS
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      //APAGAR TENDA
      Map_write_matrix(GameMap, Xi+1, '.');
      Pop(InfoStack);
    }
  }

  //colocar tenda à esquerda
  flag=0;
  if (Ci!=0) {
    flag=Tent_Valid(GameMap, Xi-1);
  }
  if(flag==1){
    //PUSH DECISAO E CONSEQUENCIAS NA PILHA
    Map_write_matrix(GameMap, Xi-1, 'T');
    Push(InfoStack, Xi-1, 'D');
    put_grass_around_tent_decision(GameMap, Xi-1, InfoStack);
    erro=Solve_MapL(GameMap, position+1, trees, InfoStack, PlacedTents+1);
    if(erro==1) return erro;
    if(erro==-1){
      //APAGAR RELVAS
      while(Stack_get_type(InfoStack, Stack_get_index(InfoStack)-1)!='D'){
        Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
        Pop(InfoStack);
      }
      //APAGAR TENDA
      Map_write_matrix(GameMap, Xi-1, '.');
      Pop(InfoStack);
    }
  }

  grass=put_grass_around_tree_decision(GameMap, Xi, InfoStack);
  erro=Solve_MapL(GameMap, position+1, trees, InfoStack, PlacedTents);
  if(erro==1) return erro;
  if(erro==-1){
    //APAGAR RELVAS
    for(i=0; i<grass; i++){
      Map_write_matrix(GameMap, Stack_get_Xi(InfoStack, Stack_get_index(InfoStack)-1), '.');
      Pop(InfoStack);
    }
  }

  return erro;
}
