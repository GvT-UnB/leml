#ifndef FRAMEMANAGER_H_INCLUDED
#define FRAMEMANAGER_H_INCLUDED
#ifdef FRAMEMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "macros.h"

/** \brief Retorna a instrucao apontada por PC e atualiza o valor de PC para a proxima instrucao.
 *
 * \param PC u4* Referencia para PC
 * \param cur_frame Frame*  Referencia para o frame corrente.
 * \return EXTERN u1 O codigo da instrrucao apontada por PC corrente.
 *
 */
EXTERN u1 getOpcode(u4 * PC, Frame * cur_frame);








#endif // FRAMEMANAGER_H_INCLUDED
