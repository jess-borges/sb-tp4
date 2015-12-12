#ifndef _H_LISTA
#define _H_LISTA

#define FALSE 0
#define TRUE 1

typedef int TipoVertice;
typedef int TipoPeso;

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

typedef struct {
    int *list;
    int size;
    int allocated;
    int block_size;
}TranslatedInstructions;

void FLVazia (TipoLista *lista);
short ListaVazia (TipoLista lista);
void Insere (TipoItem item, TipoLista *lista);
void InsereOrdenado (TipoItem item, TipoLista *lista);
void Retira (ApontadorL ap, TipoLista *lista, TipoItem *item);
void Imprime (TipoLista lista);

#endif
