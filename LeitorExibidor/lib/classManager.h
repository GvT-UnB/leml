#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN
#endif

#include "uReader.h"

/** \brief Funcao responsavel por ler o bytecode java e armazenar na memoria.
 *
 * \param dot_class FILE* arquivo bytecode que deve ser lido.
 * \param class_file ClassFile* Espaco no HEAP onde a classe serah armazenada.
 * \return ClassFile * retorna o endereço do bytecode salvo em memória.
 *
 */
EXTERN ClassFile * classRead(FILE * dot_class,ClassFile * class_file);

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


/** \brief Localiza o indice de onde o attribute CODE está salvo num attribute_info.
 *
 * \param attributes attribute_info* Referencia para o attribute_info.
 * \param attributes_count u2 Quantidade de attributes referenciados por attributes.
 * \return EXTERNu4
 *
 */
EXTERN u4 findAttributeCodeIndex(attribute_info * attributes, u2 attributes_count);

/** \brief Verifica se o bytecode informado tem o mesmo nome que o indicado dentro do bytecode.
 *
 * \param argv char* nome do bytecode
 * \param class_file ClassFile* espaço em memoria onde os daados da classe estão salvos.
 * \return void
 *
 */
EXTERN void verifyClassName(char * argv, ClassFile * class_file);

/** \brief Completa o vetor numberOfByteInstruction com a quantidade de bytes necessarias para cada instrução
 *
 * \param numberOfByteInstruction u1* Vetor que armazena a quantidade de bytes que cada instrução utiliza.
 * \return void
 *
 */
EXTERN void fillNumberOfByteInstruction(u1 * numberOfByteInstruction);

/** \brief Retorna o nome da classe do Objeto referenciado por handler.
 *
 * \param handler ClassHandler* Referencia para um objeto.
 * \return EXTERN u1* Nome da classe.
 *
 */
EXTERN u1 * getClassName(ClassFile * class_file);

/** \brief Cria o array de field estaticos e seta os valores como zero
 *
 * \param class_file ClassFile* Referencia para o espaco de memoria onde a classe serah armazenada.
 * \return EXTERN void
 *
 */
EXTERN void setStaticFields(ClassFile * class_file);

/** \brief Carrega no HEAP a classe indicada pelo arquivo de nome file_name
 *
 * \param class_file ClassFile* Referencia para o espaco de memoria onde a classe serah armazenada.
 * \param file_name char* Nome do arquivo bytecode java.
 * \param numberOfClassesHeap u4* Quantidade de classes armazenadas no HEAP. Eh incrementado ao fim da operacao.
 * \return EXTERN void
 *
 */
EXTERN void classLoader(ClassFile * class_file, char * file_name, u4 * numberOfClassesHeap);


/** \brief Procure a classe de nome className no HEAP.
 *
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4 Numero de classes armazenadas no HEAP.
 * \param className u1* Nome da classe que se deseja procurar.
 * \return EXTERN u4 Caso a clase tenha sido encontrada, eh o indice dela no HEAP. Caso contrario, retorna o erro de classe nao encontrada.
 *
 */
EXTERN u4 seekClassInHeap(ClassFile * class_file,u4 numberOfClassesHeap,u1 * className);

/** \brief Se a classe de nome className nao estiver no HEAP, ela eh carregada no HEAP e instanciada.
 *
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4* Quantidade de classes no HEAP.
 * \param className u1* Nome da classe que deve ser carregada.
 * \param handler ClassHandler* Referencia para o espaco de memoria alocado para os objetos.
 * \param numberOfClasses u4* Quantidade de objetos na memoria.
 * \return EXTERN u4 O indice da Classe no HEAP.
 *
 */
EXTERN u4 loadNewClass(ClassFile * class_file,u4 * numberOfClassesHeap,u1 * className,ClassHandler * handler,u4 * numberOfClasses,StructFrameStack *frameStackTop,u1 * numberOfByteInstruction);

/** \brief Procura pelo indice do metodo de nome method_name no method_info da classe referenciada pelo Handler
 *
 * \param handler ClassHandler* Referencia para a classe alocada no HEAP.
 * \param method_name u1* Nome do metodo.
 * \return EXTERN u4 Indice do metodo no method_info da Classe
 *
 */
EXTERN u4 seekNewMethodInClassHandler(ClassHandler * handler, u1 * method_name, u1 * method_descriptor);

/** \brief Procura o indice de um metodo no class file.
 *
 * \param class_file ClassFile* referencia para o class file
 * \param method_name u1* nome do metodo que deve ser procurado.
 * \return EXTERN u4 indice do metodo no method_info da classe.
 *
 */
EXTERN u4 seekNewMethodInClassFile(ClassFile * class_file, u1 * method_name, u1 * method_descriptor);
#endif // CLASSMANAGER_H_INCLUDED
