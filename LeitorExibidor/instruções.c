#define CLASSMANAGER_SERV
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "lib/instruções.h"


void instr_getstatic(){//FIELDREF
	u2 index, ntIndex;
	u4 cIndex, fIndex;
	u8 aux_u8;
	char *cName, *ntName, *ntType

	frame.pc++;
	index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
	cIndex = frame.cp[index].Fieldref.class_index;
	ntIndex = frame.cp[index].Fieldref.name_and_type_index;
	ntName = frame.cp[frame.cp[ntIndex].NameAndType.name_and_type_index].UTF8.bytes;
	ntType = frame.cp[frame.cp[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

}

void instr_invokeVirtual(){//METHODREF
	u2 index, ntIndex;
	u4 cIndex, fIndex;
	u8 aux_u8;
	char *cName, *mName, *mDesc

	frame.pc++;
	index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
	cIndex = frame.cp[index].Methodref.class_index;
	ntIndex = frame.cp[index].Methodref.name_and_type_index;
	mName = frame.cp[frame.cp[ntIndex].NameAndType.name_and_type_index].UTF8.bytes;
	mDesc = frame.cp[frame.cp[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

	if((strcmp(cName, "java/io/PrintStream") == 0) &&((strcmp(mdName,"print") == 0) ||(strcmp(mName,"println") == 0))){
		if(strstr(mDesc, "J") != NULL){
		} else if(strstr(mDesc, "Z") != NULL) {
		} else if(strstr(mDesc, "C") != NULL) {
		} else if(strstr(mDesc, "[C") != NULL) {
		} else if(strstr(mDesc, "I") != NULL) {
		} else if(strstr(mDesc, "F") != NULL) {
		} else if(strstr(mDesc, "Ljava/lang/String") != NULL) {

		if(strstr(mName, "println") != NULL) {
			printf("\n");
		}
	}
}

void instruction(Frame * frame, u4 pc, u1 fWide){
	u1 aux_u1, aux2_u1;
	u2 aux_u2, index;
	u4 aux_u4, aux2_u4, aux3_u4, aux4_u4;
	u8 aux_u8, aux2_u8;
	float aux_f, aux2_f;
	double aux_d, aux2_d;
	switch(frame.code[frame.pc]){
		case OPCODE_nop:
			frame.pc++;
			break;
		case OPCODE_aconst_null:
			push(NULL);         //TESTE: VER SE ISSO FUNCIONA
			frame.pc++;
			break;
		case OPCODE_iconst_m1:
			push(-1);
			frame.pc++;
			break;
		case OPCODE_iconst_0:
			push(0);
			frame.pc++;
			break;
		case OPCODE_iconst_1:
			push(1);
			frame.pc++;
			break;
		case OPCODE_iconst_2:
			push(2);
			frame.pc++;
			break;
		case OPCODE_iconst_3:
			push(3);
			frame.pc++;
			break;
		case OPCODE_iconst_4:
			push(4);
			frame.pc++;
			break;
		case OPCODE_iconst_5:
			push(5);
			frame.pc++;
			break;
		case OPCODE_lconst_0:
			push(0);
			push(0);
			frame.pc++;
			break;
		case OPCODE_lconst_1:
			push(0);
			push(1);
			frame.pc++;
			break;
		case OPCODE_fconst_0:
			aux_f = 0.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_fconst_1:
			aux_f = 1.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			break;
		case OPCODE_fconst_2:
			aux_f = 2.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dconst_0:
			aux_double = 0.0;
			memcpy(aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;
			push(aux_u4);
			aux_u4 = aux_u8;
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dconst_1:
			aux_double = 1.0;
			memcpy(aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;
			push(aux_u4);
			aux_u4 = aux_u8;
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_bipush: //TESTE: ver se isso funfa
			frame.pc++;
			aux_u1 = frame.code[frame.pc];
			aux_u4 = aux_u1;
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_sipush:
			aux_u2 = (frame.code[frame.pc+1] << 8) | frame.code[frame.pc+2];
			frame.pc++;
			frame.pc++;
			aux_u4 = aux_u2;
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_ldc:
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u1 = frame.cp[index];
			switch(aux_u1){
				case(CONSTANT_Integer):
					push(frame.cp[index].Integer.bytes)
					break
				case(CONSTANT_Float):
					push(frame.cp[index].Float.bytes)
					break
				case(CONSTANT_String)://TESTE: tem que ver isso aqui
					aux_u4 = frame.cp[index].String.string_index;
					push()
					break
			}
			frame.pc++;
			break;
		case OPCODE_ldc_w:
			frame.pc++;
			index = (frame.code[frame.pc] << 16) | frame.code[frame.pc+1];
			frame.pc++;
			aux_u1 = frame.cp[index];
			switch(aux_u1){
				case(CONSTANT_Integer):
					push(frame.cp[index].Integer.bytes)
					break
				case(CONSTANT_Float):
					push(frame.cp[index].Float.bytes)
					break
				case(CONSTANT_String)://TESTE: tem que ver isso aqui
					aux_u4 = frame.cp[index].String.string_index;
					push()
					break
			}
			frame.pc++;
			break;
		case OPCODE_ldc2_w:
			frame.pc++;
			index = = (frame.code[frame.pc] << 16) | frame.code[frame.pc+1];
			frame.pc++;
			aux_u1 = frame.cp[index];
			switch(aux_u1){
				case(CONSTANT_Long):
					push(frame.cp[index].Long.high_bytes)
					push(frame.cp[index].Long.low_byte)
					break
				case(CONSTANT_Double):
					push(frame.cp[index].Double.high_bytes)
					push(frame.cp[index].Double.low_byte)
					break
			}
			frame.pc++;
			break;
		case OPCODE_iload:
			frame.pc++;
			if(fWide == 1){
				index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
				frame.pc++;
			}
			else
				index = frame.code[frame.pc];
			push(frame.LVA[index]);
			frame.pc++;
			break;
		case OPCODE_lload:
			frame.pc++;
			if(fWide == 1){
				index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
				frame.pc++;
			}
			else
				index = frame.code[frame.pc];
			push(frame.LVA[index]);
			push(frame.LVA[index+1]);
			frame.pc++;
			break;
		case OPCODE_fload:
			frame.pc++;
			if(fWide == 1){
				index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
				frame.pc++;
			}
			else
				index = frame.code[frame.pc];
			push(frame.LVA[index]);
			frame.pc++;
			break;
		case OPCODE_dload:
			frame.pc++;
			if(fWide == 1){
				index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
				frame.pc++;
			}
			else
				index = frame.code[frame.pc];
			push(frame.LVA[index]);
			push(frame.LVA[index+1]);
			frame.pc++;
			break;
		case OPCODE_aload:
			frame.pc++;
			if(fWide == 1){
				index = (frame.code[frame.pc] << 8) | frame.code[frame.pc+1];
				frame.pc++;
			}
			else
				index = frame.code[frame.pc];
			push(frame.LVA[index]);
			frame.pc++;
			break;
		case OPCODE_iload_0:
			push(frame.LVA[0]);
			frame.pc++;
			break;
		case OPCODE_iload_1:
			push(frame.LVA[1]);
			frame.pc++;
			break;
		case OPCODE_iload_2:
			push(frame.LVA[2]);
			frame.pc++;
			break;
		case OPCODE_iload_3:
			push(frame.LVA[3]);
			frame.pc++;
			break;
		case OPCODE_lload_0:
			push(frame.LVA[0]);
			push(frame.LVA[1]);
			frame.pc++;
			break;
		case OPCODE_lload_1:
			push(frame.LVA[1]);
			push(frame.LVA[2]);
			frame.pc++;
			break;
		case OPCODE_lload_2:
			push(frame.LVA[2]);
			push(frame.LVA[3]);
			frame.pc++;
			break;
		case OPCODE_lload_3:
			push(frame.LVA[3]);
			push(frame.LVA[4]);
			frame.pc++;
			break;
		case OPCODE_fload_0:
			push(frame.LVA[0]);
			frame.pc++;
			break;
		case OPCODE_fload_1:
			push(frame.LVA[1]);
			frame.pc++;
			break;
		case OPCODE_fload_2:
			push(frame.LVA[2]);
			frame.pc++;
			break;
		case OPCODE_fload_3:
			push(frame.LVA[3]);
			frame.pc++;
			break;
		case OPCODE_dload_0:
			push(frame.LVA[0]);
			push(frame.LVA[1]);
			frame.pc++;
			break;
		case OPCODE_dload_1:
			push(frame.LVA[1]);
			push(frame.LVA[2]);
			frame.pc++;
			break;
		case OPCODE_dload_2:
			push(frame.LVA[2]);
			push(frame.LVA[3]);
			frame.pc++;
			break;
		case OPCODE_dload_3:
			push(frame.LVA[3]);
			push(frame.LVA[4]);
			frame.pc++;
			break;
		case OPCODE_aload_0:
			push(frame.LVA[0]);
			frame.pc++;
			break;
		case OPCODE_aload_1:
			push(frame.LVA[1]);
			frame.pc++;
			break;
		case OPCODE_aload_2:
			push(frame.LVA[2]);
			frame.pc++;
			break;
		case OPCODE_aload_3:
			push(frame.LVA[3]);
			frame.pc++;
			break;
		case OPCODE_iaload:
			break;
		case OPCODE_laload:
			break;
		case OPCODE_faload:
			break;
		case OPCODE_daload:
			break;
		case OPCODE_aaload:
			break;
		case OPCODE_baload:
			break;
		case OPCODE_caload:
			break;
		case OPCODE_saload:
			break;
		case OPCODE_istore:
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u4 = pop();
			frame.LVA[index] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_lstore: //TESTE: ver ordem correta
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u4 = pop();
			frame.LVA[index+1] = aux_u4; //low
			aux_u4 = pop();
			frame.LVA[index] = aux_u4; //high
			frame.pc++;
			break;
		case OPCODE_fstore:
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u4 = pop();
			frame.LVA[index] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_dstore: //TESTE: ver ordem correta
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u4 = pop();
			frame.LVA[index] = aux_u4; //low
			aux_u4 = pop();
			frame.LVA[index+1] = aux_u4; //high
			frame.pc++;
			break;
		case OPCODE_astore:
			frame.pc++;
			index = frame.code[frame.pc];
			aux_u4 = pop();
			frame.LVA[index] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_istore_0:
			aux_u4 = pop();
			frame.LVA[0] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_istore_1:
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_istore_2:
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_istore_3:
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_lstore_0:
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			aux_u4 = pop();
			frame.LVA[0] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_lstore_1:
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_lstore_2:
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_lstore_3:
			aux_u4 = pop();
			frame.LVA[4] = aux_u4;
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_fstore_0:
			aux_u4 = pop();
			frame.LVA[0] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_fstore_1:
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_fstore_2:
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_fstore_3:
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_dstore_0:
			aux_u4 = pop();
			frame.LVA[0] = aux_u4;
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_dstore_1:
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_dstore_2:
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_dstore_3:
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			aux_u4 = pop();
			frame.LVA[4] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_astore_0:
			aux_u4 = pop();
			frame.LVA[0] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_astore_1:
			aux_u4 = pop();
			frame.LVA[1] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_astore_2:
			aux_u4 = pop();
			frame.LVA[2] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_astore_3:
			aux_u4 = pop();
			frame.LVA[3] = aux_u4;
			frame.pc++;
			break;
		case OPCODE_iastore:
			break;
		case OPCODE_lastore:
			break;
		case OPCODE_fastore:
			break;
		case OPCODE_dastore:
			break;
		case OPCODE_aastore:
			break;
		case OPCODE_bastore:
			break;
		case OPCODE_castore:
			break;
		case OPCODE_sastore:
			break;
		case OPCODE_pop:
			pop();
			frame.pc++;
			break;
		case OPCODE_pop2:
			pop();
			pop();
			frame.pc++;
			break;
		case OPCODE_dup:
			aux_u4 = pop();
			push(aux_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dup_x1:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4);
			push(aux2_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dup_x2:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux3_u4 = pop();
			push(aux_u4);
			push(aux3_u4);
			push(aux2_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dup2:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux2_u4);
			push(aux_u4);
			push(aux2_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dup2_x1:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux3_u4 = pop();
			push(aux2_u4);
			push(aux_u4);
			push(aux3_u4);
			push(aux2_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dup2_x2:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux3_u4 = pop();
			aux4_u4 = pop();
			push(aux2_u4);
			push(aux_u4);
			push(aux4_u4)
			push(aux3_u4);
			push(aux2_u4);
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_swap:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_iadd:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4 + aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_ladd:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 += aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_fadd:
			aux_u4 = pop();
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux2_u4 = pop();
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f += aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dadd:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux2_d, &aux2_u8, sizeof(u8))
			aux_d += aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_isub:
			aux2_u4 = pop();
			aux_u4 = pop();
			push(aux_u4 - aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_lsub:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 -= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_fsub:
			aux2_u4 = pop();
			aux_u4 = pop();
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f -= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dsub:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8))
			aux_d -= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_imul:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4 * aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_lmul:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 *= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_fmul:
			aux2_u4 = pop();
			aux_u4 = pop();
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f *= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_dmul:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8))
			aux_d *= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_idiv:
			aux2_u4 = pop();
			aux_u4 = pop();
			push(aux_u4 / aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_ldiv:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 /= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_fdiv:
			aux2_u4 = pop();
			aux_u4 = pop();
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f /= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_ddiv:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8))
			aux_d /= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_irem:
			aux2_u4 = pop();
			aux_u4 = pop();
			push(aux_u4 % aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_lrem:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 %= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_frem:
			aux2_u4 = pop();
			aux_u4 = pop();
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f %= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_drem:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8))
			aux_d %= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_ineg:
			aux_u4 = pop();
			aux_u4 = -aux_u4;
			push(aux_u4);
			frame.pc++;
			break;
		case OPCODE_lneg:
			break;
		case OPCODE_fneg:
			break;
		case OPCODE_dneg:
			aux_u4 = pop();
			aux2_u4 = pop();
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_d = -aux_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			push(aux_u4);
			push(aux2_u4);
			frame.pc++;
			break;
		case OPCODE_ishl:
			aux_u4 = pop();
			aux_u4 &= 0x1f; // sao os menores 6 bits = 64, mas o max = 32, logo so seta os menores 5;
			aux2_u4 = pop();
			aux2_u4 <<= aux_u4;
			push(aux_u4)
			break;
		case OPCODE_lshl:
			break;
		case OPCODE_ishr:
			break;
		case OPCODE_lshr:
			break;
		case OPCODE_iushr:
			break;
		case OPCODE_lushr:
			break;
		case OPCODE_iand:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4 & aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_land:
			break;
		case OPCODE_ior:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4 | aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_lor:
			break;
		case OPCODE_ixor:
			aux_u4 = pop();
			aux2_u4 = pop();
			push(aux_u4 ^ aux2_u4 );
			frame.pc++;
			break;
		case OPCODE_lxor:

			break;
		case OPCODE_iinc:
			frame.pc++;
			aux_u1 = frame.code[frame.pc];
			aux_u4 = frame.LVA[aux_u1.pc];
			frame.pc++;
			aux2_u4 = frame.code[frame.pc];
			frame.LVA[aux_u1] = aux_u4 + aux2_u4;
			frame.pc++;
			break;
		case OPCODE_i2l:
			break;
		case OPCODE_i2f:
			break;
		case OPCODE_i2d:
			break;
		case OPCODE_l2i:
			break;
		case OPCODE_l2f:
			break;
		case OPCODE_l2d:
			break;
		case OPCODE_f2i:
			break;
		case OPCODE_f2l:
			break;
		case OPCODE_f2d:
			break;
		case OPCODE_d2i:
			break;
		case OPCODE_d2l:
			break;
		case OPCODE_d2f:
			break;
		case OPCODE_i2b:
			break;
		case OPCODE_i2c:
			break;
		case OPCODE_i2s:
			break;
		case OPCODE_lcmp:
			break;
		case OPCODE_fcmpl:
			break;
		case OPCODE_fcmpg:
			break;
		case OPCODE_dcmpl:
			break;
		case OPCODE_dcmpg:
			break;
		case OPCODE_ifeq:
			break;
		case OPCODE_ifne:
			break;
		case OPCODE_iflt:
			break;
		case OPCODE_ifge:
			break;
		case OPCODE_ifgt:
			break;
		case OPCODE_ifle:
			break;
		case OPCODE_if_icmpeq:
			break;
		case OPCODE_if_icmpne:
			break;
		case OPCODE_if_icmplt:
			break;
		case OPCODE_if_icmpge:
			break;
		case OPCODE_if_icmpgt:
			break;
		case OPCODE_if_icmple:
			break;
		case OPCODE_if_acmpeq:
			break;
		case OPCODE_if_acmpne:
			break;
		case OPCODE_goto:
			break;
		case OPCODE_jsr:
			break;
		case OPCODE_ret:
			break;
		case OPCODE_tableswitch:
			break;
		case OPCODE_lookupswitch:
			break;
		case OPCODE_ireturn:
			break;
		case OPCODE_lreturn:
			break;
		case OPCODE_freturn:
			break;
		case OPCODE_dreturn:
			break;
		case OPCODE_areturn:
			break;
		case OPCODE_return:
			break;
		case OPCODE_getstatic:
			break;
		case OPCODE_putstatic:
			break;
		case OPCODE_getfield:
			break;
		case OPCODE_putfield:
			break;
		case OPCODE_invokevirtual:
			break;
		case OPCODE_invokespecial:
			break;
		case OPCODE_invokestatic:
			break;
		case OPCODE_invokeinterface:
			break;
		case OPCODE_new:
			break;
		case OPCODE_newarray:
			break;
		case OPCODE_anewarray:
			break;
		case OPCODE_arraylength:
			break;
		case OPCODE_athrow:
			break;
		case OPCODE_checkcast:
			break;
		case OPCODE_instanceof:
			break;
		case OPCODE_monitorenter:
			break;
		case OPCODE_monitorexit:
			break;
		case OPCODE_wide:
            fWide = 1;
            frame.pc++;
			break;
		case OPCODE_multianewarray:
			break;
		case OPCODE_ifnull:
			break;
		case OPCODE_ifnonnull:
			break;
		case OPCODE_goto_w:
			break;
		case OPCODE_jsr_w:
			break;
		case OPCODE_breakpoint:
			break;
	}

}
