#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "montador.h"

/* Symbol table operations */
short searchLabel (char *string, int str_size, TipoLista st, int *address, Label *l){
    Label label;
    ApontadorL ap = st.primeiro;
    if (ap == NULL){
        return FALSE;
    }
    while((ap != NULL) && !ListaVazia(st)){
        label = ap->item;
        if (isEqual(string, str_size, label.name, label.name_size)){
            *address = label.address;
            *l = label;
            return TRUE;
        }
        ap = ap->prox;
    }
    return FALSE;
}

short defineLabel(char *string, int str_size, TipoLista *st, int address){
    Label label;
    ApontadorL ap = st->primeiro;
    if (ap == NULL){
        return FALSE;
    }
    while((ap != NULL) && !ListaVazia(*st)){
        label = ap->item;
        if (isEqual(string, str_size, label.name, label.name_size)){
            if (!label.defined){
                label.address = address;
                ap->item = label;
                return TRUE;
            }
            else{
                return FALSE;
            }
        }
        ap = ap->prox;
    }

    copyLabelName(&label, string, str_size);
    label.address = address;
    label.defined = TRUE;
    Insere(label, st);
    return 0;
}

Label addUndefinedLabel(char *string, int str_size, TipoLista *st, TranslatedInstructions *code, int pc){
    Label label;
    int address;

    copyLabelName(&label, string, str_size);
    label.defined = FALSE; 
    if (!searchLabel(label.name, label.name_size, *st, &address, &label)){
        Insere(label, st);
    }
    return label;
}

short isUndefinedLabel(TranslatedInstructions code, int pc){
    if (code.undefinedLabels[pc].name_size != 0){
        return TRUE;
    }
    else{
        return FALSE;
    }
    
}
void writeUndefinedLabel(TranslatedInstructions code, int pc, FILE **file){
    fprintf(*file, "%s\n", code.undefinedLabels[pc].name);
}

/* Translation operations*/
void translate(char *in_file, char *out_file, short verbose, short linker){
    TipoLista symbol_table;
    
    firstStep(in_file, &symbol_table);    
    secondStep(in_file, out_file, &symbol_table, verbose, linker);
    
    if (verbose){
        Imprime(symbol_table);
    }
}

void firstStep(char *in_file, TipoLista *st){
    char c;
    char *word;
    int i, pc, allocated;
    short lastCharInWord;
    FILE *file;

    FLVazia(st);
    file = fopen(in_file, "r");
    if (!file){
        printf("\nHouve um problema com o arquivo");
        exit(0);
    }
    pc = 0;
    lastCharInWord = FALSE;
    c = '*';
    
    allocated = BLOCK_SIZE;
    word = (char *) malloc(allocated*sizeof(char));    
    while (!feof(file)){
        i = 0;
        c = getc(file);
        while((c != ':') && !feof(file)){
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){ /* Reading the label */
                
                word[i] = c;                
                i++;
                lastCharInWord = TRUE;
                if (i >= 10){
                    allocated = allocated + BLOCK_SIZE;
                    word = (char *) realloc(word, allocated*sizeof(char));
                }                
            }
            else{
                if (((c == ' ') || (c == '\n') || (c == '\t')) && lastCharInWord && !isEqualInstr(word, i, WORD) && !isEqualInstr(word, i, END)){                    
                    pc++; 
                }
                i = 0;
                if (c == ';'){/* Ignoring the comments */
                    while (c != '\n'){
                        c = getc(file);
                    }
                }
                lastCharInWord = FALSE;
            }
            c = getc(file);
            
        }
        
        if (c == ':'){
            defineLabel(word, i, st, pc);
            lastCharInWord = FALSE;
        }
    }    
    free(word);
    fclose(file);
}

void secondStep(char *in_file, char *out_file, TipoLista *st, short verbose, short linker){
    FILE *file;
    char c, *word;
    int allocated, i, pc;
    short lastCharInWord, expecting;
    TranslatedInstructions code;

    file = fopen(in_file, "r");
    if (!file){
        printf("\nHouve um problema na abertura do arquivo");
        exit(0);
    }

    StartISACode(&code, 5*BLOCK_SIZE);

    allocated = BLOCK_SIZE;
    word = (char *) malloc(allocated*sizeof(char));

    pc = 0;
    lastCharInWord = FALSE;
    expecting = INSTRUCTION;

    while (!feof(file)){
        i = 0;
        c = getc(file);
        while((c != ' ') && (c != '\n') && (c != '\t') && !feof(file)){
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
                word[i] = c;                
                i++;
                lastCharInWord = TRUE;
                if (i >= allocated){
                    allocated = allocated + BLOCK_SIZE;
                    word = (char *) realloc(word, allocated*sizeof(char));
                }
            }
            else{
                i = 0;
                lastCharInWord = FALSE;

                if (c == ';'){/* Ignoring the comments */
                    while (c != '\n'){
                        c = getc(file);
                    }
                }
            }
            c = getc(file);
        }
        if (((c == ' ') || (c == '\n') || (c == '\t')) && (lastCharInWord)){
            expecting = translatesWord(word, i, &code, st, expecting, pc);
            if ((expecting != VALUE) && (expecting != END)){
                pc++;
            }            
            lastCharInWord = FALSE;
        }
    }
    printCode(out_file, code, linker, *st);
    fclose(file);
}

int translatesWord(char *word, int wsize, TranslatedInstructions *code, TipoLista *st, short expecting, int pc){
    int i, n;
    Label label;
    i = 0;
    while ((i <= END) && (expecting == INSTRUCTION)){        
        if (isEqualInstr(word, wsize, i)){       
            if (i == WORD){
                return VALUE;
            }
            if (i == END){
                return END;
            }
            saveInListCode(code, i, FALSE, label);
            if ((i == RET) || (i == HALT)){ // zero operands
                return INSTRUCTION;
            }
            if ((i == READ) || (i == WRITE) || (i == NEG) || (i == NOT) || (i == PUSH) || (i == POP)){ 
                return OPERAND_1;
            }
            if ( (i == COPY) || ((i >= SUB) && (i <= XOR)) ){
                return OPERAND_2;
            }
            if ((i == LOAD) || (i == STORE)){
                return OPERAND_LABEL;
            }
            if (  ((i >= JMP) && (i <= JNN)) || (i == CALL)  ){
                return LABEL;                
            }
        }
        else{
            i++;
        }
    }
    if ((expecting == OPERAND_1) || (expecting == OPERAND_2) || (expecting == OPERAND_LABEL)){
        if ((word[0] != 'R') || (wsize < 2) || (wsize > 3)){
            printf("\nErro no Operando de registrador");
            return ERROR;
        }        
        if (wsize == 3){
            n = atoiChar(word[2]);
            n = n + 10*(atoiChar(word[1]));
        }
        else{               
            n = atoiChar(word[1]);
        }
        saveInListCode(code, n, FALSE, label);

        expecting--;
        return expecting;
    }
    if (expecting == LABEL){
        if (searchLabel(word, wsize, *st, &n, &label)){
            if (label.defined){
                n = n - pc - 1;
                saveInListCode(code, n, FALSE, label);
            }   
            else{
                saveInListCode(code, pc, TRUE, label);
            }         
            return INSTRUCTION;
        }
        else{
            label = addUndefinedLabel(word, wsize, st, code, pc);
            saveInListCode(code, pc, TRUE, label);
            return INSTRUCTION;
        }
    }
    if (expecting == VALUE){
        n = wordToInt(word, wsize);
        saveInListCode(code, n, FALSE, label);
        return INSTRUCTION;
    }
    return ERROR;
}

short printCode(char *out_file, TranslatedInstructions code, short linker, TipoLista st){
    FILE *file;
    int i;

    file = fopen(out_file, "w");
    if (!file){
        printf("\nHouve um problema na abertura do arquivo");
        return 0;
    }
    if (linker){
        ImprimeEmArquivo(st, &file);
        fprintf(file, "[%d]\n", code.size);
    }    
    for (i = 0; i < code.size; i++){
        if (code.list[i] == i){
            if (isUndefinedLabel(code, i)){
                writeUndefinedLabel(code, i, &file);
            }
        }
        else{
            fprintf(file, "%d\n", code.list[i]);
        }
    }
    fclose(file);
    return 1;
}

/* TranslatedInstructions operations */
void StartISACode(TranslatedInstructions *code, int blockSize){
    code->allocated = blockSize;
    code->ulallocated = blockSize;
    code->block_size = blockSize;
    code->list = (int *) malloc(code->allocated*sizeof(int));
    code->size = 0;
    code->undefinedLabels = (Label *) malloc(code->ulallocated*sizeof(Label));
    code->ulsize = 0;
}

void saveInListCode(TranslatedInstructions *code, int n, short undefinedLabel, Label label){
    if (code->size == code->allocated){
        ReallocISACode(code);
    }
    code->list[code->size] = n;
    code->size++;

    if (!undefinedLabel){    
        label.name = "";
        label.name_size = 0;
    }
    code->undefinedLabels[code->ulsize] = label;
    code->ulsize++;
    if (code->ulsize >= code->ulallocated){
        code->ulallocated += code->block_size;
        code->undefinedLabels = (Label *) realloc(code->undefinedLabels, code->ulallocated*sizeof(Label));
    }
}
void ReallocISACode(TranslatedInstructions *code){
    code->allocated += code->block_size;
    code->list = (int *) realloc(code->list, code->allocated*sizeof(int));
}

void FreeISACode(TranslatedInstructions *code){
    code->allocated = 0;
    code->size = 0;
    free(code->list);
}

/* String and label operations */
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

short isEqualInstr (char *str1, int str_size1, int j){
    int i;
    const char* instructions[] = {"WORD", "LOAD", "STORE", "READ", "WRITE", "COPY", "NEG", "SUB", "ADD", "AND", "OR", "XOR", "NOT", 
                                  "JMP", "JZ", "JNZ", "JN", "JNN", "PUSH", "POP", "CALL", "RET", "HALT", "END" };
    const int stringInstrSize[] = {4, 4, 5, 4, 5, 4, 3, 3, 3, 3, 2, 3, 3, 3, 2, 3, 2, 3, 4, 3, 4, 3, 4, 3};
    if (str_size1 != stringInstrSize[j]){
        return 0;
    }
    for (i = 0; i < stringInstrSize[j]; i++){
        if (str1[i] != instructions[j][i]){
            
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

void printLabel(Label label, int size){
    int i;
    printf("\nLabel: [");
    for (i = 0; i < size; i++){
        printf("%c", label.name[i]);
    }
    printf("] - %d\n", label.address);
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
