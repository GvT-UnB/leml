#define EXCEPTIONMANAGER_SERV
#include "lib/exceptionManager.h"
#include <stdio.h>
#include <stdarg.h>

void throwException(int exception_code, char * string_error, ...){
    va_list args;
    va_start(args, string_error);
    fprintf(stderr, "ERROR %d: ",exception_code); //Printa o codigo de erro no temrinal
    vprintf(string_error, args); //Printa no terminal a mensagem de erro juntamente de todos os argumentos que possam ter sido passados à função.
    fprintf(stderr, "\nFinalizando o programa...\n");
    va_end(args);
    exit(exception_code); //Encerra o programa retornando o codigo de erro.
}
