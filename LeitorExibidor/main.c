#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/UserViewer.h"

int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    char *program_full_name = argv[0]; /* nome do programa para caso de erro */

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        throwException(OPEN_FILE_ERROR,OPEN_FILE_ERROR_MSG);
    }
    printf("Lendo o bytecode Java para a memoria...\n");
    class_file = classRead(dot_class);
    classPrint(dot_class, class_file);
    fclose(dot_class);
    return 0;
}
