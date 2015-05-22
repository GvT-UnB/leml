#ifndef UREADER_H_INCLUDED
#define UREADER_H_INCLUDED
#ifdef UREADER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdio.h>
#include "macros.h"



EXTERN u1 u1Read(FILE * fp);

EXTERN u2 u2Read(FILE * fp);

EXTERN u4 u4Read(FILE * fp);

#endif // UREADER_H_INCLUDED
