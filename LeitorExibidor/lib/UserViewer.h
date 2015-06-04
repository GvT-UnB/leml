#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif


#include "macros.h"
/** \brief Funcao responsavel por imprimir na tela as informacoes dos atributos.
 *
 * \param ponteiros para o atributo a ser impresso e para o constant_pool, class_file e a quantidade de atributos a serem impressos.
 * \return void
 *
 */
EXTERN void printAttributesInfo(attribute_info * attributes, u2 attributes_count, cp_info * constant_poo, ClassFile *class_file);
/** \brief Principal funcao responsavel por imprimir o bytecode.
 *
 * \param fp FILE* ponteiro para o arquivo bytecode e class_file estrutura principal.
 * \return void
 *
 */
EXTERN void classPrint(FILE * dot_class, ClassFile * class_file);
/** \brief Principal funcao responsavel por pegar a string referenciada pelos ponteiros presentes tanto no constant_pool, methods,\
            attributes e fields.
 *
 * \param ponteiro para o class_file, o indice no constant_pool buscado e o ponteiro para a string.
 * \return void
 *
 */
EXTERN void catchString(ClassFile *class_file, int i, char *string);
/** \brief funcao que seleciona em qual ponteiro vai ser buscado a string. Caso, so tenha um ponteiro, nao faz diferenca
 *
 * \param ponteiro para o class_file, o indice no constant_pool buscado e o ponteiro para a string e a opcao de qual ponteiro a ser usado.
 * \return void
 *
 */
EXTERN void selectPointer(ClassFile *class_file, int i, char *string, int option);
/** \brief funcao que seleciona em qual ponteiro vai ser buscado a string. Caso, so tenha um ponteiro, nao faz diferenca
 *
 * \param ponteiro para o class_file, o indice no constant_pool buscado e o ponteiro para a string e a opcao de qual ponteiro a ser usado.
 * \return void
 *
 */


EXTERN void printCode(u1 *code, u4 code_length,cp_info * constant_pool);
/** \brief funcao que imprimi o bytecode do atributo code.
 *
 * \param ponteiro para o bytecode do atributo code, tamanho do bytecode e o ponteiro da constant pool.
 * \return void
 *
 */

EXTERN void getAccessFlag(int flag, char *string, int tipo);
#endif // CLASSMANAGER_H_INCLUDED
