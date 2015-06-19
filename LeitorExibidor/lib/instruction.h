#ifndef CLASSMANAGER_H_INCLUDED
#define CLASSMANAGER_H_INCLUDED
#ifdef CLASSMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include "macros.h"

EXTERN void doInstructionShift(Frame *cur_frame, u1 curOPCODE, u4 *curPC, StructFrameStack *frameStackTop);

EXTERN void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code );

EXTERN void instr_tableSwitch(Frame *frame, u4 pc, u1 * code);

EXTERN void instr_lookUpSwitch(Frame *frame, u4 pc, u1 * code);

#endif // CLASSMANAGER_H_INCLUDED
