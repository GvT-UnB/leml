#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include "lib/macros.h"
//#include "lib/uReader.h"
#include "lib/classManager.h"

void classRead(FILE * dot_class);

int main(int argc, char *argv[]){
    FILE * dot_class;
    char *program_full_name = argv[0]; /* nome do programa para caso de erro */

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        fprintf(stderr, "%s: Nao foi possivel abrir o arquivo %s\n", program_full_name, *argv); //Em caso de erro
        exit(1);
    }

    classRead(dot_class);

    fclose(dot_class);
    return 0;
}
