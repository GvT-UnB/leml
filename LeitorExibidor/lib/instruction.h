#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "macros.h"

EXTERN u4 getFieldIndex(field_info * fields, char * fName, u2 fields_count, cp_info * constant_pool );

EXTERN void instr_putstatic(Frame * frame, u4 pc, u1 * code);

EXTERN void instr_getstatic(Frame * frame, u4 pc, u1 * code);

EXTERN int16_t printBin(int16_t var, int dim);

EXTERN void instr_invokeVirtual(Frame * frame, u4 pc, u1 fWide, u1 * code);

EXTERN void doInstructionShift(Frame **cur_frame/*, u1 curOPCODE*/, u4 *curPC, StructFrameStack *frameStackTop, u1 * code, u1 fWide);

EXTERN void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code );

EXTERN void instr_tableSwitch(Frame *frame, u4 *curPC, u1 * code);

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
