#include <stdlib.h>
#include <stdio.h>
#include "montador.h"
#include "lista.h"


int main(int argc, char * argv[])
{
    short verbose, linker;
    char *input, *output;
    /*  Arguments
        0 - input file name
        1 - output file name        
        2 - output mode (optional argument): s/v (simple/verbose)
        4 - output type: -L if it's going to be used in a linker and nothing if it's not
    */
    if ((argc < 3) || (argc > 5)) {
        printf("Numero de parametros errado.");
        return -1;
    }
    input = argv[1];
    output = argv[2];
    verbose = FALSE;
    linker = FALSE;
    if (argc > 3){
        if (argv[3][0] == 'v'){
            verbose = TRUE;
        }
        else{
            if (argv[3][0] == '-' && (argv[3][1] == 'L' || argv[3][1] == 'l')){
                linker = TRUE;
            }
        }
        if (argv[4][0] == '-' && (argv[4][1] == 'L' || argv[4][1] == 'l')){
            linker = TRUE;
        }
    }

    translate(input, output, verbose, linker);
    return 0;
}

