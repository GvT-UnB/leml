#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdio.h>
#include "uReader.h"

EXTERN void classRead(FILE * dot_class);

#endif // CLASSMANAGER_H_INCLUDED
