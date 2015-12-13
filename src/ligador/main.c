#include <stdlib.h>
#include <stdio.h>
#include "ligador.h"


int main(int argc, char * argv[])
{
    Linker linker;
    int i, allocated;

    allocated = BLOCK_SIZE;
    linker.moduleList = (Module *) malloc(allocated*sizeof(Module));
    linker.size = 0;

    i = 1;
    while((i < argc) && (argv[i][0] != '-')){
        linker.moduleList[linker.size].fileName = argv[i];
        if (linker.size >= allocated){
            allocated += BLOCK_SIZE;
            linker.moduleList = (Module *) realloc(linker.moduleList, allocated*sizeof(Module));            
        }
        linker.size++;
        i++;
    }
    if ((argv[i][0] == '-') && (argv[i][1] == 'm')){
        i++;
        linker.main.fileName = argv[i];
        i++;
    }
    if ((argv[i][0] == '-') && (argv[i][1] == 'o')){
        i++;
        linker.out_file = argv[i];
    }
    link (linker);
    return 0;
}

