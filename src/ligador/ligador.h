#include <stdio.h>
#include "lista.h"

#ifndef LIGADOR_H
#define LIGADOR_H

#define BLOCK_SIZE 5

/* Ligador definitions*/
typedef struct {
    TipoLista symbolTable;
    char *fileName;
    int finalPC;
} Module;

typedef struct {
    Module *moduleList;
    int size;

    Module main;
    char *out_file;
} Linker;

void link (Linker linker);
short searchLabelInTable (char *string, int str_size, TipoLista st, int *address);
int searchLabel (char *string, int str_size, Linker linker);
void readTables (Linker *linker);
void readTable (Module *module);
void sumPCs(TipoLista *st, int pc);
void rearrangesTables (Linker *linker);
void fillsTable (TipoLista *st, Linker linker);
void fillsUndefinedLabels (Linker *linker);
void write (Linker linker);
void writeModule (Module module, FILE **out);

short isEqual (char *str1, int str_size1, char *str2, int str_size2);
Label copyLabelName(char *src, int size);
int wordToInt(char *word, int wsize);
int power(int n, int exponent);
int atoiChar(char c);

#endif
