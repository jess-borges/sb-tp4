#include "ligador.h"
#include "lista.h"
#include <string.h>
#include <stdio.h>

void link (Linker linker){
    /*int i;
    FILE *file;*/
    readTables(&linker);
    rearrangesTables(&linker);
    fillsUndefinedLabels(&linker);
    write(linker);

    /* Para gerar um qrquivo com as tabelas de símbolos 
    de todos os módulos e do programa principal, descomente. 
    */
    /*file = fopen("tst/tabelas", "w");

    
    ImprimeEmArquivo (linker.main.symbolTable, &file);
    for (i = 0; i < linker.size; i++){
        ImprimeEmArquivo (linker.moduleList[i].symbolTable, &file);
    }
    fclose(file);*/
}

short searchLabelInTable (char *string, int str_size, TipoLista st, int *address){
    Label label;
    ApontadorL ap = st.primeiro;
    if (ap == NULL){
        return FALSE;
    }
    while((ap != NULL) && !ListaVazia(st)){
        label = ap->item;
        if (isEqual(string, str_size, label.name, label.name_size)){
            if (label.defined){
                *address = label.address;
                return TRUE;
            }
        }
        ap = ap->prox;
    }
    return FALSE;
}

int searchLabel (char *string, int str_size, Linker linker){
    int i, address;

    address = 0;
    for (i = 0; i < linker.size; i++){
        if (searchLabelInTable(string, str_size, linker.moduleList[i].symbolTable, &address)){
            return address;
        }
    }
    searchLabelInTable(string, str_size, linker.main.symbolTable, &address);
    return address;
}

void readTables (Linker *linker){
    int i;

    readTable(&linker->main);
    for (i = 0; i < linker->size; i++){
        readTable(&linker->moduleList[i]);
    }
}

void readTable (Module *module){
    FILE *file;
    char c, *word;
    Label label;
    int i, allocated;


    FLVazia(&module->symbolTable);
    allocated = 10;
    word = (char *) malloc(allocated*sizeof(char));

    file = fopen(module->fileName, "r");
    if (!file){
        printf("\nHouve um problema com o arquivo %s", module->fileName);
        exit(0);
    }

    c = ' ';
    while (!feof(file) && c != '{'){
        c = getc(file);
    }
    c = getc(file); /* Reads '\n' */
    c = getc(file);
    while (!feof(file) && c != '}'){
        
        i = 0;
        while (c != ' ' && c != '\n' && !feof(file)){
            word[i] = c;
            i++;
            c = getc(file);
            if (i >= allocated){
                allocated += 10;
                word = (char *) realloc(word, allocated*sizeof(char));
            }
        }
        if (c == ' '){
            label.name_size = i;            
            label.name = (char *) malloc((i+5)*sizeof(char));
            label.name = strncpy(label.name, word, i);
        }
        i = 0;
        c = getc(file);
        while (c != ' ' && c != '\n' && !feof(file)){
            word[i] = c;
            i++;
            c = getc(file);
            if (i >= allocated){
                allocated += 10;
                word = (char *) realloc(word, allocated*sizeof(char));
            }
        }
        if (c == '\n'){
            if (word[0] != '#'){
                label.address = wordToInt(word, i);
                label.defined = TRUE;
            }
            else{
                label.defined = FALSE;
            }
        }
        Insere(label, &module->symbolTable);
        c = getc(file);
    }
    c = getc(file); /* reads \n */
    c = getc(file);
    if (c == '['){
        i = 0;
        c = getc (file);
        while (c != ']' && !feof(file)){
            word[i] = c;
            i++;
            c = getc(file);
            if (i >= allocated){
                allocated += 10;
                word = (char *) realloc(word, allocated*sizeof(char));
            }
        }
        if (c == ']'){
            module->finalPC = wordToInt(word, i);
        }
    }
    fclose(file);
    free(word);
}

void rearrangesTables (Linker *linker){
    int i, currentPC;
    
    currentPC = linker->main.finalPC;
    for (i = 0; i < linker->size; i++){
        IncrementaEnderecos(&linker->moduleList[i].symbolTable, currentPC);
        currentPC += linker->moduleList[i].finalPC;
    }
}

void fillsTable (TipoLista *st, Linker linker){
    ApontadorL ap;
    Label label;
    int address;

    ap = st->primeiro->prox;
    if (ap == NULL){
        return;
    }
    
    while ((ap != NULL) && !ListaVazia(*st)){
        label = ap->item;
        if(!ap->item.defined){
            address = searchLabel (label.name, label.name_size, linker);
            label.address = address;
            label.defined = TRUE;
            ap->item = label;
        }
        ap = ap->prox;
    }
}

void fillsUndefinedLabels (Linker *linker){
    int i;
    fillsTable(&linker->main.symbolTable, *linker);
    for (i = 0; i < linker->size; i++){
        fillsTable(&linker->moduleList[i].symbolTable, *linker);
    }
}

void write (Linker linker){
    FILE *file;
    int i;

    file = fopen (linker.out_file, "w");
    if (!file){
        printf("\nHouve um problema na abertura do arquivo");
        return;
    }

    writeModule(linker.main, &file);
    for (i = 0; i < linker.size; i++){
        writeModule(linker.moduleList[i], &file);
    }
    fclose(file);
}
void writeModule (Module module, FILE **out){
    FILE *in;
    char c, *word;
    int i, allocated, address;
    

    in = fopen(module.fileName, "r");
    if (!in){
        printf("\nHouve um problema na abertura do arquivo");
        return;
    }
    allocated = 10;
    word = (char *) malloc(allocated*sizeof(char));
    while (!feof(in) && c != ']'){ /* Reads informations of symbol table */
        c = getc(in);
    }
    c = getc(in); /* Reads '\n' */
    while (!feof(in)){
        c = getc(in);
        if (c >= 'A' && c <= 'Z'){
            i = 0;
            while (c != ' ' && c != '\n' && !feof(in)){
                word[i] = c;
                i++;
                c = getc(in);
                if (i >= allocated){
                    allocated += 10;
                    word = (char *) realloc(word, allocated*sizeof(char));
                }
            }
            searchLabelInTable(word, i, module.symbolTable, &address);
            fprintf(*out, "%d", address);
        }
        if (c == ' ' || c == '\n' || (c >= '0' && c <= '9'))
            fprintf(*out, "%c", c);
    }
    free (word);
    fclose(in);
}

short isEqual (char *str1, int str_size1, char *str2, int str_size2){
    int i;
    if (str_size1 != str_size2){
        return 0;
    }
    for (i = 0; i < str_size1; i++){
        if (str1[i] != str2[i]){
            return 0;
        }
    }
    return 1;
}

Label copyLabelName(char *src, int size){
    int i;
    Label label;
    label.name = (char *) malloc(size*sizeof(char));
    for (i = 0; i < size; i++){
        label.name[i] = src[i];
    }
    label.name_size = size;

    printf("{%s- %d}",  label.name, label.name_size);
    return label;
}


int wordToInt(char *word, int wsize){
    int i, n, aux;
    n = 0;
    for (i = wsize-1; i >= 0; i--){        
        aux = atoiChar(word[i]);        
        n = n + ((power(10, wsize - 1 - i))*aux);
    }
    return n;
}

int power(int n, int exponent){
    int i;
    if (exponent == 0){
        return 1;
    }
    for (i = 1; i < exponent; i++){
        n = n*n;
    }
    return n;
}

int atoiChar(char c){
    int n;
    n = (int) c;
    n = n - 48;
    return n;
}
