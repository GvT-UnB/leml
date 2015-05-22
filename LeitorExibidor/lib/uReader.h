#ifndef UREADER_H_INCLUDED
#define UREADER_H_INCLUDED
#ifdef UREADER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdio.h>
#include "macros.h"



/** \brief Função responsavel por ler um byte do bytecode java.
 *
 * \param fp FILE* ponteiro para o arquivo bytecode.
 * \return EXTERNu1 byte lido.
 *
 */
EXTERN u1 u1Read(FILE * fp);

/** \brief Função responsavel por ler dois bytes do bytecode java.
 *
 * \param fp FILE* ponteiro para o arquivo bytecode.
 * \return EXTERNu1 Os bytes lidos.
 *
 */
EXTERN u2 u2Read(FILE * fp);

/** \brief Função responsavel por ler quatro bytes do bytecode java.
 *
 * \param fp FILE* ponteiro para o arquivo bytecode.
 * \return EXTERNu1 Os bytes lidos.
 *
 */
EXTERN u4 u4Read(FILE * fp);

#endif // UREADER_H_INCLUDED
