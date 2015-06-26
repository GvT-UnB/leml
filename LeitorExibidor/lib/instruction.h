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

EXTERN void  doInvokestatic(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

EXTERN void  doInvokespecial(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

EXTERN void doInstructionInvoke(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

EXTERN void doCLInit(ClassFile * class_file,u1 * code, Frame * clinit_frame, u1 * numberOfByteInstruction);

EXTERN void doNew(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

EXTERN void  doInvokespecial(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

EXTERN void  doInvokestatic(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

#endif // CLASSMANAGER_H_INCLUDED
