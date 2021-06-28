#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "utilities.h"
#include "SetGameMap.h"
#include "Solver.h"

//estrutura que guarda as alterações feitas ao mapa durante a sua resolução
struct Stack_Struct{
  int Xi; //posicao na matriz
  char type; //decisao ou consequencia
};

//estrutura que define a pilha
struct Stack_Info{
  StackStruct *Stack;
  int free_index;
  int stack_size;
};

// ******************************************************************************
// * Init()
// * Arguments:   stack_size - tamanho da pilha
// * Returns:     InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Description: Esta função inicializa a stack, ou seja, aloca uma pilha, aloca stack_size estruturas
//                que definem a pilha e coloca o índice a 0
// *****************************************************************************
StackInfo* Init(int stack_size){

  StackInfo *InfoStack;

  InfoStack = (StackInfo*)check_calloc(sizeof(StackInfo), 1);
  InfoStack->Stack = (StackStruct*)check_calloc(sizeof(StackStruct), stack_size);
  InfoStack->free_index=0;
  InfoStack->stack_size=stack_size;

  return InfoStack;
}

// ******************************************************************************
// * Push()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
//                Xi - celula em questão
//                type - se se trata de uma decisao (por norma colocar tenda) ou de uma
//                       consequencia de essa decisao (por norma colocar relvas)
// * Returns:     InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Description: Esta função faz push na stack, ou seja, guarda a célula, guarda se é uma decisão ou uma
//                consequencia e incrementa o indice
// *****************************************************************************
StackInfo* Push(StackInfo *InfoStack, int Xi, char type){

  InfoStack->Stack[InfoStack->free_index].Xi = Xi;
  InfoStack->Stack[InfoStack->free_index].type = type;
  InfoStack->free_index++;

  return InfoStack;
}

// ******************************************************************************
// * Pop()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Returns:     void
// * Description: Esta função faz pop na stack, ou seja, decrementa o indice
// *****************************************************************************
void Pop(StackInfo *InfoStack){

  InfoStack->free_index--;

}

// ******************************************************************************
// * Empty()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Returns:     void
// * Description: Esta função faz free da stack
// *****************************************************************************
void Empty(StackInfo *InfoStack){

  free(InfoStack->Stack);
  free(InfoStack);

}

// ******************************************************************************
// * Stack_get_Xi()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
//                i - célula em questão
// * Returns:     InfoStack->Stack[i].Xi
// * Description: Esta função retorna a posicao da célula guardada na stack
// *****************************************************************************
int Stack_get_Xi(StackInfo* InfoStack, int i){

  return InfoStack->Stack[i].Xi;
}

// ******************************************************************************
// * Stack_get_type()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
//                i - célula em questão
// * Returns:     InfoStack->Stack[i].type
// * Description: Esta função retorna se a célula da stack se trata de uma decisão ou de uma consequencia
// *****************************************************************************
char Stack_get_type(StackInfo* InfoStack, int i){

  return InfoStack->Stack[i].type;
}

// ******************************************************************************
// * Stack_get_index()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Returns:     InfoStack->free_index
// * Description: Esta função retorna o valor do index da pilha na altura em questão
// *****************************************************************************
int Stack_get_index(StackInfo* InfoStack){

  return InfoStack->free_index;
}

// ******************************************************************************
// * Stack_get_size()
// * Arguments:   InfoStack - estrutura que guarda as alteraçsões feitas ao mapa durante a sua resolução
// * Returns:     InfoStack->stack_size
// * Description: Esta função retorna o tamanho da pilha
// *****************************************************************************
int Stack_get_size(StackInfo* InfoStack){

  return InfoStack->stack_size;
}
