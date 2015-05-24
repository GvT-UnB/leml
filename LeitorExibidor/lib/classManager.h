#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "uReader.h"

/** \brief Fun��o responsavel por ler o bytecode java e armazenar na mem�ria.
 *
 * \param dot_class FILE* arquivo bytecode que deve ser lido.
 * \return ClassFile * retorna o endere�o do bytecode salvo em mem�ria.
 *
 */
EXTERN ClassFile * classRead(FILE * dot_class);

#endif // CLASSMANAGER_H_INCLUDED
