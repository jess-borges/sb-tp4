#include <stdio.h>
#include "lista.h"

#ifndef LIGADOR_H
#define LIGADOR_H

typedef struct {
    TipoLista symbolTable;
    TranslatedInstructions code;
    int finalPC;
} Module;



#endif
