#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Recebe um ponteiro para a lista e a inicializa.
 */
void FLVazia (TipoLista *lista){
    lista->primeiro = (ApontadorL) malloc(sizeof(CelulaLista));
    lista->ultimo = lista->primeiro;
    lista->primeiro->prox = NULL;
}

/*
 * Recebe uma lista e retorna 1 se está vazia e 0 caso contrário.
 */
short ListaVazia (TipoLista lista){
    return (lista.primeiro == lista.ultimo);
}

/*
 * Recebe um item e um ponteiro para a lista e insere o item na lista.
 */
void Insere (TipoItem item, TipoLista *lista){
    lista->ultimo->prox = (ApontadorL) malloc(sizeof(CelulaLista));
    lista->ultimo = lista->ultimo->prox;
    lista->ultimo->item = item;
    lista->ultimo->prox = NULL;
    /*printf("{Insere %s %d %d}", item.name, item.address, item.defined);*/
}

/*
void InsereOrdenado (TipoItem item, TipoLista *lista){
	ApontadorL ap, apAnterior, novo;
	apAnterior = lista->primeiro;
	ap = apAnterior->prox;
	while ((ap != NULL) && (ap->item.vertice < item.vertice)){
		apAnterior = ap;
		ap = ap->prox;
	}
	if (ap == NULL){
		Insere (item, lista);
		return;
	}
	novo = (ApontadorL) malloc(sizeof(CelulaLista));
	apAnterior->prox = novo;
	novo->prox = ap;	
}
*/
/*
 * Retira a célula apontada por ap da lista e guarda seu conteúdo no item, cujo ponteiro é passado por parâmetro.
 */
void Retira (ApontadorL ap, TipoLista *lista, TipoItem *item){
    ApontadorL apPosterior;
    if (ListaVazia(*lista) || ap == NULL || ap->prox == NULL){
        printf("Erro: Lista vazia ou posicao nao existe");
        return;
    }
    apPosterior = ap->prox;
    *item = apPosterior->item;
    ap->prox = apPosterior->prox;
    if (ap->prox == NULL)
        lista->ultimo = ap;
    free(apPosterior);
}

/*
 * Recebe a lista e imprime seus itens.
 */
void Imprime (TipoLista lista){
    ApontadorL ap;
    ap = lista.primeiro->prox;
    printf("\n****Tabela de simbolos****\n");
    while (ap != NULL){
        printf("\n%s: ", ap->item.name);
        if (ap->item.defined)
            printf("%d", ap->item.address);
        else
            printf("not defined");
        ap = ap->prox;
    }
    printf("\n");
}

void ImprimeEmArquivo (TipoLista lista, FILE **arq){
    ApontadorL ap;
    ap = lista.primeiro->prox;
    fprintf(*arq, "{");
    while (ap != NULL){
        fprintf(*arq, "\n%s ", ap->item.name);
        if (ap->item.defined)
            fprintf(*arq, "%d", ap->item.address);
        else
            fprintf(*arq, "#");
        ap = ap->prox;
    }
    fprintf(*arq, "\n}\n");
}