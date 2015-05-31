#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "uReader.h"

/** \brief Função responsavel por ler o bytecode java e armazenar na memória.
 *
 * \param dot_class FILE* arquivo bytecode que deve ser lido.
 * \return ClassFile * retorna o endereço do bytecode salvo em memória.
 *
 */
EXTERN ClassFile * classRead(FILE * dot_class);

EXTERN void readAttributesInfo(attribute_info * attributes, u2 attributes_count, FILE * dot_class, cp_info * constant_pool);

EXTERN void readClasses(Classes * Classes, u2 number_of_classes, FILE * dot_class);

EXTERN void readLineNumberTable(Line_number_table * line_number_table, u2 line_number_table_length, FILE * dot_class);

EXTERN void readLocalVariableTable(Local_variable_table * local_variable_table, u2 local_variable_table_length, FILE * dot_class);


#endif // CLASSMANAGER_H_INCLUDED
