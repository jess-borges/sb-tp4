#include "ligador.h"
#include "lista.h"

void link (Linker linker){
    int i;

    readTables(&linker);
    rearrangesTables(&linker);
    fillsUndefinedLabels(&linker);

    printf("\nPrograma principal\n");
    Imprime (linker.main.symbolTable);
    printf("\nPC final: %d", linker.main.finalPC);
    for (i = 0; i < linker.size; i++){
        printf("\nModulo %d\n", i+1);
        Imprime (linker.moduleList[i].symbolTable);
        printf("\nPC final: %d", linker.moduleList[i].finalPC);
    }
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
    /*printf("\nPrograma principal\n");
    Imprime (linker->main.symbolTable);*/
    for (i = 0; i < linker->size; i++){
        readTable(&linker->moduleList[i]);
        /*printf("\nModulo %d\n", i+1);
        Imprime (linker->moduleList[i].symbolTable);*/
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
            copyLabelName(&label, word, i);
            /*printf("1[%s %d]%s  ", word, i, label.name);*/
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
            /*printf("2[%s %d]", word, i);*/
            if (word[0] != '#'){
                label.address = wordToInt(word, i);
                /*printf("%d  ", label.address);*/
                label.defined = TRUE;
            }
            else{
                /*printf("  ");*/
                label.defined = FALSE;
            }
           /* c = getc(file); /* Reads '\n' */
        }
        Insere(label, &module->symbolTable);
        c = getc(file);
    }
    c = getc(file); /* reads \n */
    c = getc(file);
    printf("\n veio aqui -%c-", c);
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
            printf("3[%s %d]%d", word, i, module->finalPC);
        }
    }
    fclose(file);
    free(word);
}

void sumPCs(TipoLista *st, int pc){
}

void rearrangesTables (Linker *linker){
    int i, currentPC;
    
    currentPC = linker->main.finalPC;
    for (i = 0; i < linker->size; i++){
        sumPCs(&linker->moduleList[i].symbolTable, currentPC);
        printf("soma");
        currentPC += linker->moduleList[i].finalPC;
    }
}

void fillsTable (TipoLista *st, Linker linker){
    ApontadorL ap;
    Label label;
    int address;

    ap = st->primeiro;
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

}
void writeModule (Module module, FILE **file){

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

void copyLabelName(Label *label, char *src, int size){
    int i, allocated;
    allocated = size + (BLOCK_SIZE - (size % BLOCK_SIZE));
    label->name = (char *) malloc(allocated*sizeof(char));
    for (i = 0; i < size; i++){
        label->name[i] = src[i];
    }
    label->name_size = size;
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
