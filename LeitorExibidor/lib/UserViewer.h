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
EXTERN void printAttributesInfo(attribute_info * attributes, u2 attributes_count, cp_info * constant_poo, ClassFile *class_file, FILE* output);

/** \brief Principal funcao responsavel por imprimir o bytecode.
 *
 * \param class_file ClassFile* estrutura principal.
 * \return EXTERN void
 *
 */
EXTERN void classPrint(ClassFile * class_file);
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


EXTERN void printCode(u1 *code, u4 code_length,cp_info * constant_pool, FILE *output);
/** \brief funcao que imprimi o bytecode do atributo code.
 *
 * \param ponteiro para o bytecode do atributo code, tamanho do bytecode e o ponteiro da constant pool.
 * \return void
 *
 */

EXTERN void getAccessFlag(int flag, char *string, int tipo, FILE* output);


/** \brief Funcao que printa no dispositivo padrao de saida os dados do method_info
 *
 * \param methods method_info* Referencia para um method_info
 * \param methods_count u2 Quantidade de method_info que devem ser printados
 * \param class_file ClassFile* Referencia para a Classe dona do metodo referenciado pelo method_info
 * \return EXTERN void
 *
 */
EXTERN void printMethodInfo(method_info * methods, u2 methods_count, ClassFile *class_file, FILE* output);

/** \brief uncao que printa no dispositivo padrao de saida os dados do constant_pool
 *
 * \param constant_pool cp_info* Referencia para o constant_pool que sera printado
 * \param constant_pool_count u2 Quantidade de membros do constant_pool que sera printado
 * \param class_file ClassFile* Referencia para a classe dona do constant pool
 * \return EXTERN void
 *
 */
EXTERN void printConstantPool(cp_info * constant_pool, u2 constant_pool_count, ClassFile *class_file, FILE* file);
#endif // CLASSMANAGER_H_INCLUDED
