#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "uReader.h"

/** \brief Funcao responsavel por ler o bytecode java e armazenar na memoria.
 *
 * \param dot_class FILE* arquivo bytecode que deve ser lido.
 * \return ClassFile * retorna o endereço do bytecode salvo em memória.
 *
 */
EXTERN ClassFile * classRead(FILE * dot_class);

/** \brief Funcao responsavel por ler para a memoria os dados referentes ao Attributte Info do bytecode java.
 *
 * \param attributes attribute_info* ponteiro para o espaço de memória onde os dados devem ser salvos.
 * \param attributes_count u2 a quantidade de atributos que serão lidos do bytecode.
 * \param dot_class FILE* ponteiro para o arquivo bytecode.
 * \param constant_pool cp_info* ponteiro para o constant pool.
 * \return void
 *
 */
EXTERN void readAttributesInfo(attribute_info * attributes, u2 attributes_count, FILE * dot_class, cp_info * constant_pool);

/** \brief Funcao responsavel por ler para a memoria os dados referentes as Classes do Attribute Info.
 *
 * \param Classes Classes* ponteiro para o espaço de memória onde os dados devem ser salvos.
 * \param number_of_classes u2 quantidade de classes que serão lidas.
 * \param dot_class FILE* ponteiro para o arquivo bytecode.
 * \return EXTERN void
 *
 */
EXTERN void readClasses(Classes * Classes, u2 number_of_classes, FILE * dot_class);

/** \brief
 *
 * \param line_number_table Line_number_table* ponteiro para o espaço de memoria onde os dados devem ser salvos.
 * \param line_number_table_length u2 quantidade de line number tabes que serão lidas.
 * \param dot_class FILE* ponteiro para o arquivo bytecode.
 * \return EXTERN void
 *
 */
EXTERN void readLineNumberTable(Line_number_table * line_number_table, u2 line_number_table_length, FILE * dot_class);

/** \brief
 *
 * \param local_variable_table Local_variable_table* ponteiro para o espaço de memoria onde os dados devem ser salvos.
 * \param local_variable_table_length u2 quantidade de local variable table que serao lidas.
 * \param dot_class FILE* ponteiro para o arquivo bytecode.
 * \return EXTERN void
 *
 */
EXTERN void readLocalVariableTable(Local_variable_table * local_variable_table, u2 local_variable_table_length, FILE * dot_class);


#endif // CLASSMANAGER_H_INCLUDED
