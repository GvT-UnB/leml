#define UREADER_SERV
#include "lib/uReader.h"

 u1 u1Read(FILE * fp){
    u1 toReturn = getc(fp);
    return toReturn;
}

 u2 u2Read(FILE * fp){
    u2 toReturn = getc(fp);
    toReturn = (toReturn << BYTE_SIZE) | getc(fp);
    return toReturn;
}

 u4 u4Read(FILE * fp){
    u4 toReturn = getc(fp);
    for(int i=0;i<3;i++){
        toReturn = (toReturn << BYTE_SIZE) | getc(fp);
    }
    return toReturn;
}
