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

#endif // CLASSMANAGER_H_INCLUDED
