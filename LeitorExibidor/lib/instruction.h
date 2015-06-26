#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "macros.h"

EXTERN void doInstructionShift(Frame **cur_frame, u4 *curPC, StructFrameStack *frameStackTop, u1 * code, u1 fWide);

EXTERN void doInstructionArray(Frame * frame, u4 pc, u1 fWide, u1 * code);

EXTERN void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code );

EXTERN void instr_tableSwitch(Frame *frame, u4 *curPC, u1 * code);

EXTERN void instr_lookUpSwitch(Frame *frame, u4 *curPC, u1 * code);

EXTERN void doInstructionInvoke(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction);

#endif // CLASSMANAGER_H_INCLUDED
