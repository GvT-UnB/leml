#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include "lib/macros.h"
#include "lib/uReader.h"



int main(int argc, char *argv[]){
    FILE * dot_class;
    char *program_full_name = argv[0]; /* nome do programa para caso de erro */
    char byte;
    ClassFile class_file;

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        fprintf(stderr, "%s: Nao foi possivel abrir o arquivo %s\n", program_full_name, *argv); //Em caso de erro
        exit(1);
    }

    class_file.magic = u4Read(dot_class);
    class_file.minor_version = u2Read(dot_class);
    class_file.major_version = u2Read(dot_class);
    class_file.constant_pool_count = u2Read(dot_class);

    printf("Magic Number: 0x%08x\n",class_file.magic);
    printf("Minor Version: %d\n",class_file.minor_version);
    printf("Major Version: %d\n",class_file.major_version);
    printf("Constant Pool Counter: %d\n",class_file.constant_pool_count);

    fclose(dot_class);
    return 0;
}
