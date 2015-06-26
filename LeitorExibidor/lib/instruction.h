#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "macros.h"

/** \brief pega o index do field baseado no indice dele no field_info
 *
 * \param fields field_info* vetor com os fields da classe
 * \param fName char* resolucao do nome do field
 * \param fields_count u2 quantidade de fields
 * \param constant_pool cp_info* ponteiro pra constant pool
 * \return EXTERNu4 retorna o fieldIndex
 *
 */
EXTERN u4 getFieldIndex(field_info * fields, char * fName, u2 fields_count, cp_info * constant_pool );

/** \brief Executa a intrucao putstatic
 *
 * \param frame Frame* ponteiro pro frame sendo executado
 * \param pc u4 pc atual
 * \param code u1* vetor de codigos
 * \return EXTERN void
 *
 */
EXTERN void instr_putstatic(Frame * frame, u4 pc, u1 * code);

/** \brief Executa a intrucao getstatic
 *
 * \param frame Frame* ponteiro pro frame sendo executado
 * \param pc u4 pc atual
 * \param code u1* vetor de codigos
 * \return EXTERN void
 *
 */
EXTERN void instr_getstatic(Frame * frame, u4 pc, u1 * code);

/** \brief printa um numero binario em decimal
 *
 * \param var int16_t variavel binaria
 * \param dim int varivel decimal
 * \return EXTERN int16_t retorno  numero binario
 *
 */
EXTERN int16_t printBin(int16_t var, int dim);

/** \brief Executa a intrucao getstatic
 *
 * \param frame Frame* ponteiro pro frame sendo executado
 * \param pc u4 pc atual
 * \param fWide u1 flag wide
 * \param code u1* vetor de codigos
 * \return EXTERN void
 *
 */
EXTERN void instr_invokeVirtual(Frame * frame, u4 pc, u1 fWide, u1 * code);

/** \brief Funcao responsavel pela implentacao de todas as instrucoes que necessitam de deslocamento dentro do codigo
 *
 * \param cur_frame Frame** Ponteiro do frame corrente
 * \param curPC u4* Pc corrente
 * \param frameStackTop StructFrameStack* Referencia ao topo da pilha de frames
 * \param code u1* Referencia ao bytecode
 * \param fWide u1 Flag para a instrucao wide
 * \return EXTERN void
 *
 */
EXTERN void doInstructionShift(Frame **cur_frame, u4 *curPC, StructFrameStack *frameStackTop, u1 * code, u1 fWide);

/** \brief Funcao referente a todas as instrucoes que nao necessitam de deslocamento e nem array
 *
 * \param frame Frame* Referencia ao frame corrente
 * \param pc u4 copia do pc corrente
 * \param fWide u1 Flag para checar se o fWide esta setado
 * \param code u1* Bytecode
 * \return EXTERN void
 *
 */
EXTERN void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code );

/** \brief Instrucao referente ao tableSwitch
 *
 * \param frame Frame* Referencia ao frame corrente
 * \param curPC u4* ponteiro para o pc corrente
 * \param code u1* bytecode
 * \return EXTERN void
 *
 */
EXTERN void instr_tableSwitch(Frame *frame, u4 *curPC, u1 * code);

/** \brief Intrucao referente ao lookUpSwich
 *
 * \param frame Frame* Referencia para o frame corrente
 * \param curPC u4* Ponteiro para o pc corrente
 * \param code u1*  ponteiro para o bytecode
 * \return EXTERN void
 *
 */
EXTERN void instr_lookUpSwitch(Frame *frame, u4 *curPC, u1 * code);

/** \brief Executa a instrucao Invokestati
 *
 * \param cur_frame Frame* Referencia para o frame corrente.
 * \param frameStackTop StructFrameStack* Referencia para a pilha de frames.
 * \param handler ClassHandler* Referencia para o Objeto dono do metodo.
 * \param curPC u4 PC corrente.
 * \param flagIsWide u1 Flag que informa se a instrucao eh do tipo wide.
 * \param code u1* Referencia para o code
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4* Numero de classes no HEAP
 * \param numberOfClasses u4* Numero de Classes instanciadas.
 * \param numberOfByteInstruction u1* Quantidade de bytes usados para cada instrucao
 * \return EXTERN void
 *
 */
EXTERN void  doInvokestatic(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

/** \brief Realiza a instrucao Invokespecial
 *
 * \param cur_frame Frame* Referencia para o frame corrente.
 * \param frameStackTop StructFrameStack* Referencia para a pilha de frames.
 * \param handler ClassHandler* Referencia para o Objeto dono do metodo.
 * \param curPC u4 PC corrente.
 * \param flagIsWide u1 Flag que informa se a instrucao eh do tipo wide.
 * \param code u1* Referencia para o code
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4* Numero de classes no HEAP
 * \param numberOfClasses u4* Numero de Classes instanciadas.
 * \param numberOfByteInstruction u1* Quantidade de bytes usados para cada instrucao
 * \return EXTERN void
 *
 */
EXTERN void  doInvokespecial(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

/** \brief Realiza as instrucoes do tipo Invoke ou New
 *
 * \param cur_frame Frame* Referencia para o frame corrente.
 * \param frameStackTop StructFrameStack* Referencia para a pilha de frames.
 * \param handler ClassHandler* Referencia para o Objeto dono do metodo.
 * \param curPC u4 PC corrente.
 * \param flagIsWide u1 Flag que informa se a instrucao eh do tipo wide.
 * \param code u1* Referencia para o code
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4* Numero de classes no HEAP
 * \param numberOfClasses u4* Numero de Classes instanciadas.
 * \param numberOfByteInstruction u1* Quantidade de bytes usados para cada instrucao
 * \return EXTERN void
 *
 */
EXTERN void doInstructionInvoke(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

/** \brief Realiza a instrucao <clinit>
 *
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param code u1* Referencia para o code
 * \param clinit_frame Frame* Referencia para o frame do clinit.
 * \param numberOfByteInstruction u1* Quantidade de bytes usados para cada instrucao
 * \return EXTERN void
 *
 */
EXTERN void doCLInit(ClassFile * class_file,u1 * code, Frame * clinit_frame, u1 * numberOfByteInstruction);

/** \brief Realiza a operacao New.
 *
 * \param cur_frame Frame* Referencia para o frame corrente.
 * \param frameStackTop StructFrameStack* Referencia para a pilha de frames.
 * \param handler ClassHandler* Referencia para o Objeto dono do metodo.
 * \param curPC u4 PC corrente.
 * \param code u1* Referencia para o code
 * \param class_file ClassFile* Referencia para o HEAP.
 * \param numberOfClassesHeap u4* Numero de classes no HEAP
 * \param numberOfClasses u4* Numero de Classes instanciadas.
 * \param numberOfByteInstruction u1* Quantidade de bytes usados para cada instrucao
 * \return EXTERN void
 *
 */
EXTERN void doNew(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

#endif // CLASSMANAGER_H_INCLUDED
