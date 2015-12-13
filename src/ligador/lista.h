#include <stdio.h>
#include <stdlib.h>

#ifndef _H_LISTA
#define _H_LISTA

#define FALSE 0
#define TRUE 1


typedef struct {
    char *name;
    int name_size;
    int address;
    short defined;
} Label;

typedef Label TipoItem;

typedef struct CelulaL_struct *ApontadorL;

typedef struct CelulaL_struct{
    TipoItem item;
    ApontadorL prox;
} CelulaLista;

typedef struct{
    ApontadorL primeiro, ultimo;
} TipoLista;




/*
typedef struct {
    int *list;
    int size;
    int allocated;
    int block_size;
    int *undefinedLabels; / Records the positions of undefined labels /
    int ulsize; / Size of undefinedLabels array /
    int ulallocated;
}TranslatedInstructions;
*/
void FLVazia (TipoLista *lista);
short ListaVazia (TipoLista lista);
void Insere (TipoItem item, TipoLista *lista);
void InsereOrdenado (TipoItem item, TipoLista *lista);
void Retira (ApontadorL ap, TipoLista *lista, TipoItem *item);
void IncrementaEnderecos (TipoLista *lista, int inc);
void Imprime (TipoLista lista);
void ImprimeEmArquivo (TipoLista lista, FILE **arq);


#endif
