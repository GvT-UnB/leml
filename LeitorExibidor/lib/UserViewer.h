#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif


#include "macros.h"

EXTERN void printAttributesInfo(attribute_info * attributes, u2 attributes_count, cp_info * constant_poo, ClassFile *class_file);

EXTERN void classPrint(FILE * dot_class, ClassFile * class_file);

EXTERN void catchString(ClassFile *class_file, int i, char *string);

EXTERN void selectPointer(ClassFile *class_file, int i, char *string, int option);

EXTERN u4 getFloat(u4 number);

EXTERN void printCode(u1 *code, u4 code_length,cp_info * constant_pool);

EXTERN void getAccessFlag(int flag, char *string, int tipo);
#endif // CLASSMANAGER_H_INCLUDED
