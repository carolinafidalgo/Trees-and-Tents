#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "SetGameMap.h"
#include "Solver.h"

// ******************************************************************************
// * check_calloc()
// * Arguments:   size - tamanho finito de memória a alocar
//                n - contante a multipliciar
// * Returns:     ptr - ponteiro para o espaço de memória alocado
// * Description: Esta função faz calloc e verifica-o
// *****************************************************************************
void *check_calloc(size_t size, size_t n) {
  void* ptr = calloc(size, n);

  if(ptr == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(0);
  }
  return ptr;
}

// ******************************************************************************
// * check_arguments()
// * Arguments:   argv[]
// * Returns:     void
// * Description: Esta função confirma a validade dos argumentos de entrada no programa
// *****************************************************************************
void check_arguments(char* argv[]){

  char aux[7]="\0";

  strcpy(aux, argv[1]+(strlen(argv[1])-5));
  if(strcmp(aux, ".camp")!=0){
    fprintf(stderr, "Second argument invalid\n");
    exit(0);
  }
}
