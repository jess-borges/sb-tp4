#include <stdio.h>
#include "lista.h"

#ifndef _MONTADOR
#define _MONTADOR

#define FALSE 0
#define TRUE  1

#define MEMORY_SIZE 1000
#define REGISTER_NUMBER 16

/*                        Virtual Machine Instructions                            */ 
#define LOAD  1 /* Operands: R M   .... Reg[R] <- Mem[M+PC]                       */
#define STORE 2 /* Operands: R M   .... Mem[M+PC] <- Reg[R]                       */

#define READ  3 /* Operands: R     .... Reg[R] <- input                           */
#define WRITE 4 /* Operands: R     .... output <- Reg[R]                          */
#define COPY  5 /* Operands: R1 R2 .... Reg[R1] <- Reg[R2]             $          */

#define NEG   6 /* Operands: R     .... Reg[R] <- -Reg[R]              $          */
#define SUB   7 /* Operands: R1 R2 .... Reg[R1] <- Reg[R1] - Reg[R2]   $          */
#define ADD   8 /* Operands: R1 R2 .... Reg[R1] <- Reg[R1] + Reg[R2]   $          */
#define AND   9 /* Operands: R1 R2 .... Reg[R1] <- Reg[R1] AND Reg[R2] $          */
#define OR   10 /* Operands: R1 R2 .... Reg[R1] <- Reg[R1] OR Reg[R2]  $          */
#define XOR  11 /* Operands: R1 R2 .... Reg[R1] <- Reg[R1] XOR Reg[R2] $          */
#define NOT  12 /* Operands: R     .... Reg[R] <- NOT Reg[R]           $          */

#define JMP  13 /* Operands: M     .... PC <- PC + M                              */
#define JZ   14 /* Operands: M     .... if PSW[zero], PC <- PC + M                */
#define JNZ  15 /* Operands: M     .... if !PSW[zero], PC <- PC + M               */
#define JN   16 /* Operands: M     .... if PSW[negativo], PC <- PC + M            */
#define JNN  17 /* Operands: M     .... if !PSW[negativo], PC <- PC + M           */

#define PUSH 18 /* Operands: R     .... SP <- SP - 1; Mem[SP] <- Reg[R]           */
#define POP  19 /* Operands: R     .... Reg[R] <- Mem[SP]; SP <- SP + 1           */

#define CALL 20 /* Operands: M     .... SP <- SP - 1; Mem[SP] <- PC; PC <- PC + M */
#define RET  21 /* Operands: none  .... PC <- Mem[SP]; SP <- SP + 1               */

#define HALT 22 /* Operands: none  .... Stop                                      */
/* The symbol $ means the instruction modifies PSW                                */

/*                             Pseudoinstructions                                 */ 
#define WORD  0 /* Operands: I     .... Mem[PC] <- I                              */
#define END  23 /* Operands: none  .... Ends the translation                      */

#define BLOCK_SIZE 10

#define INSTRUCTION 0
#define OPERAND_1 1
#define OPERAND_2 2
#define LABEL 3
#define OPERAND_LABEL 4
#define VALUE 5
#define ERROR -1

short searchLabel (char *string, int str_size, TipoLista st, int *address, Label *l);
short defineLabel(char *string, int str_size, TipoLista *st, int address);
Label addUndefinedLabel(char *string, int str_size, TipoLista *st, TranslatedInstructions *code, int pc);
short isUndefinedLabel(TranslatedInstructions code, int pc);
void writeUndefinedLabel(TranslatedInstructions code, int pc, FILE **file);

void translate(char *in_file, char *out_file, short verbose, short linker);
void firstStep(char *in_file, TipoLista *st);
void secondStep(char *in_file, char *out_file, TipoLista *st, short verbose, short linker);
int translatesWord(char *word, int wsize, TranslatedInstructions *code, TipoLista *st, short expecting, int pc);
short printCode(char *out_file, TranslatedInstructions code, short linker, TipoLista st);

void StartISACode(TranslatedInstructions *code, int blockSize);
void saveInListCode(TranslatedInstructions *code, int n, short undefinedLabel, Label label);
void ReallocISACode(TranslatedInstructions *code);
void FreeISACode(TranslatedInstructions *code);

short isEqual (char *str1, int str_size1, char *str2, int str_size2);
short isEqualInstr (char *str1, int str_size1, int j);
void copyLabelName(Label *label, char *src, int size);
void printLabel(Label label, int size);
int wordToInt(char *word, int wsize);
int power(int n, int exponent);
int atoiChar(char c);

#endif
