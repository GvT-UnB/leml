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
 * \return EXTERN u1 O codigo da instrucao apontada por PC corrente.
 *
 */
EXTERN u1 getOpcode(u4 * PC, Frame * cur_frame);

/** \brief Copia o conteudo de attributesSrc para attributes
 *
 * \param attributes attribute_info* Vetor destino de attribute_info
 * \param attributes_count u2 Tamanho do vetor
 * \param attributesSrc attribute_info* Vetor fonte de attribute_info
 * \param constant_pool cp_info * referencia para o constant pool da classe
 * \return void
 *
 */
EXTERN void copyAttributesInfo(attribute_info * attributes, u2 attributes_count, attribute_info * attributesSrc, cp_info * constant_pool);

/** \brief Copia o conteudo de classesSrc para classesDst
 *
 * \param classesDst Classes* Vetor destino de Classes
 * \param number_of_classes u2 Tamanho dos vetores
 * \param classesSrc Classes* Vetor fonte de Classes
 * \return void
 *
 */
EXTERN void copyClasses(Classes * classesDst, u2 number_of_classes, Classes * classesSrc);

/** \brief Copia o conteudo de line_number_table_src para line_number_table_dst
 *
 * \param line_number_table_dst Line_number_table*  Vetor destino de Line Number Table
 * \param line_number_table_length u2 Tamanho do vetor
 * \param line_number_table_src Line_number_table* Vetor fonte de Line Number Table
 * \return void
 *
 */
EXTERN void copyLineNumberTable(Line_number_table * line_number_table_dst, u2 line_number_table_length, Line_number_table * line_number_table_src);

/** \brief Copia o conteudo de local_variable_table_src para local_variable_table_dst
 *
 * \param local_variable_table_dst Local_variable_table* Vetor destino de Local Variable Table
 * \param local_variable_table_length u2 Tamanho do vetor
 * \param local_variable_table_src Local_variable_table* Vetor fonte de Local Variable Table
 * \return void
 *
 */
EXTERN void copyLocalVariableTable(Local_variable_table * local_variable_table_dst, u2 local_variable_table_length, Local_variable_table * local_variable_table_src);

/** \brief Instancia um novo objeto da classe class_file
 *
 * \param handler ClassHandler* Novo Objeto
 * \param class_file ClassFile* Referencia para a classe
 * \return void
 *
 */
EXTERN void newObject(ClassHandler * handler, ClassFile * class_file);

/** \brief Empilha um operando na pilha de operandos
 *
 * \param  operandStackTop structOperandStack** Ponteiro para o topo da pilha
 * \param  operand u4 Operando a ser colocado na pilha
 * \return void
 *
 */
EXTERN void pushOperandStack(structOperandStack **operandStackTop, u4 operand);

/** \brief Retira um operando da pilha e retorna o operando retirado
 *
 * \param operandStackTop structOperandStack** Ponteiro para o topo da pilha
 * \return u4 retorna o operando retirado da pilha
 *
 */
EXTERN u4 popOperandStack(structOperandStack **operandStackTop);

#endif // FRAMEMANAGER_H_INCLUDED
