#define CLASSMANAGER_SERV
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

//#include "lib/instruction.h"
#include "lib/macros.h"
/*
void instr_getstatic(){//FIELDREF
	u2 index, ntIndex;
	u4 cIndex, fIndex;
	u8 aux_u8;
	char *cName, *ntName, *ntType;

	pc++;
	index = (code[pc] << 8) | code[pc+1];
	cIndex = frame->constant_pool[index].Fieldref.class_index;
	ntIndex = frame->constant_pool[index].Fieldref.name_and_type_index;
	ntName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_and_type_index].UTF8.bytes;
	ntType = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

}

void instr_invokeVirtual(){//METHODREF
	u2 index, ntIndex;
	u4 cIndex, fIndex;
	u8 aux_u8;
	char *cName, *mName, *mDesc;

	pc++;
	index = (code[pc] << 8) | code[pc+1];
	cIndex = frame->constant_pool[index].Methodref.class_index;
	ntIndex = frame->constant_pool[index].Methodref.name_and_type_index;
	mName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_and_type_index].UTF8.bytes;
	mDesc = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

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
*/
void doInstructionReturn(Frame *cur_frame, u1 curOPCODE, u4 *curPC, StructFrameStack *frameStackTop){
}
void instr_tableSwitch(Frame *frame, u4 pc, u1 * code){
    u1 byte1, byte2, byte3, byte4;
    u4 highTableS, lowTableS, defaultTableS, valTableS, opcodeAddress, targetAddress, *tableSwitch;
    tableSwitch = (u4*)malloc(sizeof(u4)); ///Aloca memória para a tabela(vetor) do switch
    opcodeAddress = pc;
    while((pc + 1)%4 != 0){ ///Loop para o preenchimento do padding <0-3 bytes>
        pc++;
    }
    pc++;

    byte1 = code[pc++];
    byte2 = code[pc++];
    byte3 = code[pc++];
    byte4 = code[pc++];
    defaultTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[pc++];
    byte2 = code[pc++];
    byte3 = code[pc++];
    byte4 = code[pc++];
    lowTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[pc++];
    byte2 = code[pc++];
    byte3 = code[pc++];
    byte4 = code[pc++];
    highTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    valTableS = popOperandStack(frame->operandStack);

    for(int i; i < highTableS-lowTableS+1; i++){ /// Preenche a tabela (vetor) com os offsets de cada case
        byte1 = code[pc++];
        byte2 = code[pc++];
        byte3 = code[pc++];
        byte4 = code[pc++];
        tableSwitch[i] = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    }

    if(valTableS < lowTableS || valTableS > highTableS){
        targetAddress = opcodeAddress + defaultTableS;
    }
    else{
        targetAddress = opcodeAddress + tableSwitch[valTableS-lowTableS];
    }
    pc = targetAddress; /// PC e mandado para o offset selecionado
}

void instr_lookUpSwitch(Frame *frame, u4 pc, u1 * code){
    u1 byte1, byte2, byte3, byte4, found;
    u4 *lookupSwitch, opcodeAddress, defaultLS, targetAddress, npair, i, key;
    opcodeAddress = pc;
    while((pc + 1)%4 != 0){
        pc++;
    }
    pc++;

    byte1 = code[pc++];
    byte2 = code[pc++];
    byte3 = code[pc++];
    byte4 = code[pc++];
    defaultLS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[pc++];
    byte2 = code[pc++];
    byte3 = code[pc++];
    byte4 = code[pc++];
    npair = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; ///Quantidade de itens a serem percorridos

    lookupSwitch = (u4*)malloc(npair*2*sizeof(u4)); /// armazena em um vetor o dobro do espaço de variaveis do switch para armazenar os valores dentro deles

    for(int i=0; i< npair*2; i += 2){ ///Loop anda de dois em dois para pegar dois valores de uma vez
        byte1 = code[pc++];
        byte2 = code[pc++];
        byte3 = code[pc++];
        byte4 = code[pc++];
        lookupSwitch[i] = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; /// match

        byte1 = code[pc++];
        byte2 = code[pc++];
        byte3 = code[pc++];
        byte4 = code[pc++];
        lookupSwitch[i+1] = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; /// offset (key)
    }

    key = popOperandStack(); ///recupera o valor do topo da pilha
    i=0;
    while( found != 1 && i < ((npair*2)-1)){///o loop para quando ele chega no limite do array - 1 ou quando encontra o match
        if(key != lookupSwitch[i]){
            found = 1; /// Flag para quando o match é encontrado
        }
        i+=2; ///incrementa de dois em dois para buscar entre os match's
    }
    if(found == 0){/// caso nao encontre
        targetAddress = defaultLS + opcodeAddress;
    }
    else{///caso encontre
        targetAddress = lookupSwitch[i+1] + opcodeAddress; ///pega o offset seguido do match selecionado
    }
    pc = targetAddress;
}

void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code ){
	u1 aux_u1, aux2_u1;
	u2 aux_u2, index, branchoffset;
	u4 aux_u4, aux2_u4, aux3_u4, aux4_u4, returnAddress;
	u8 aux_u8, aux2_u8;
	float aux_f, aux2_f;
	double aux_d, aux2_d, aux_double;

	switch(code[pc]){
		case OPCODE_nop:
			pc++;
			break;
		case OPCODE_aconst_null:
			pushOperandStack( frame->operandStack, NULL);         //TESTE: VER SE ISSO FUNCIONA
			pc++;
			break;
		case OPCODE_iconst_m1:
			pushOperandStack( frame->operandStack, -1);
			pc++;
			break;
		case OPCODE_iconst_0:
			pushOperandStack( frame->operandStack, 0);
			pc++;
			break;
		case OPCODE_iconst_1:
			pushOperandStack( frame->operandStack, 1);
			pc++;
			break;
		case OPCODE_iconst_2:
			pushOperandStack( frame->operandStack, 2);
			pc++;
			break;
		case OPCODE_iconst_3:
			pushOperandStack( frame->operandStack, 3);
			pc++;
			break;
		case OPCODE_iconst_4:
			pushOperandStack( frame->operandStack, 4);
			pc++;
			break;
		case OPCODE_iconst_5:
			pushOperandStack( frame->operandStack, 5);
			pc++;
			break;
		case OPCODE_lconst_0:
			pushOperandStack( frame->operandStack, 0);
			pushOperandStack( frame->operandStack, 0);
			pc++;
			break;
		case OPCODE_lconst_1:
			pushOperandStack( frame->operandStack, 0);
			pushOperandStack( frame->operandStack, 1);
			pc++;
			break;
		case OPCODE_fconst_0:
			aux_f = 0.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_fconst_1:
			aux_f = 1.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			break;
		case OPCODE_fconst_2:
			aux_f = 2.0;
			memcpy(aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dconst_0:
			aux_double = 0.0;
			memcpy(aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;
			pushOperandStack( frame->operandStack, aux_u4);
			aux_u4 = aux_u8;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dconst_1:
			aux_double = 1.0;
			memcpy(aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;
			pushOperandStack( frame->operandStack, aux_u4);
			aux_u4 = aux_u8;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_bipush: //TESTE: ver se isso funfa
			pc++;
			aux_u1 = code[pc];
			aux_u4 = aux_u1;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_sipush:
			aux_u2 = (code[pc+1] << 8) | code[pc+2];
			pc++;
			pc++;
			aux_u4 = aux_u2;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_ldc:
			pc++;
			index = code[pc];
			aux_u1 = frame->constant_pool[index].tag;
			switch(aux_u1){
				case(CONSTANT_Integer):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Integer.bytes);
					break;
				case(CONSTANT_Float):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Float.bytes);
					break;
				case(CONSTANT_String)://TESTE: tem que ver isso aqui
					aux_u4 = frame->constant_pool[index].String.string_index;
					//pushOperandStack( frame->operandStack, );
					break;
			}
			pc++;
			break;
		case OPCODE_ldc_w:
			pc++;
			index = (code[pc] << 16) | code[pc+1];
			pc++;
			aux_u1 = frame->constant_pool[index].tag;
			switch(aux_u1){
				case(CONSTANT_Integer):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Integer.bytes);
					break;
				case(CONSTANT_Float):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Float.bytes);
					break;
				case(CONSTANT_String)://TESTE: tem que ver isso aqui
					aux_u4 = frame->constant_pool[index].String.string_index;
					//pushOperandStack( frame->operandStack, );
					break;
			}
			pc++;
			break;
		case OPCODE_ldc2_w:
			pc++;
			index = (code[pc] << 16) | code[pc+1];
			pc++;
			aux_u1 = frame->constant_pool[index].tag;
			switch(aux_u1){
				case(CONSTANT_Long):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Long.high_bytes);
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Long.low_bytes);
					break;
				case(CONSTANT_Double):
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Double.high_bytes);
					pushOperandStack( frame->operandStack, frame->constant_pool[index].Double.low_bytes);
					break;
			}
			pc++;
			break;
		case OPCODE_iload:
			pc++;
			if(fWide == 1){
				index = (code[pc] << 8) | code[pc+1];
				pc++;
			}
			else
				index = code[pc];
			pushOperandStack( frame->operandStack, frame->localVariableArray[index].value);
			pc++;
			break;
		case OPCODE_lload:
			pc++;
			if(fWide == 1){
				index = (code[pc] << 8) | code[pc+1];
				pc++;
			}
			else
				index = code[pc];
			pushOperandStack( frame->operandStack, frame->localVariableArray[index].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[index+1].value);
			pc++;
			break;
		case OPCODE_fload:
			pc++;
			if(fWide == 1){
				index = (code[pc] << 8) | code[pc+1];
				pc++;
			}
			else
				index = code[pc];
			pushOperandStack( frame->operandStack, frame->localVariableArray[index].value);
			pc++;
			break;
		case OPCODE_dload:
			pc++;
			if(fWide == 1){
				index = (code[pc] << 8) | code[pc+1];
				pc++;
			}
			else
				index = code[pc];
			pushOperandStack( frame->operandStack, frame->localVariableArray[index].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[index+1].value);
			pc++;
			break;
		case OPCODE_aload:
			pc++;
			if(fWide == 1){
				index = (code[pc] << 8) | code[pc+1];
				pc++;
			}
			else
				index = code[pc];
			pushOperandStack( frame->operandStack, frame->localVariableArray[index].value);
			pc++;
			break;
		case OPCODE_iload_0:
			pushOperandStack( frame->operandStack, frame->localVariableArray[0].value);
			pc++;
			break;
		case OPCODE_iload_1:
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pc++;
			break;
		case OPCODE_iload_2:
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pc++;
			break;
		case OPCODE_iload_3:
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pc++;
			break;
		case OPCODE_lload_0:
			pushOperandStack( frame->operandStack, frame->localVariableArray[0].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pc++;
			break;
		case OPCODE_lload_1:
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pc++;
			break;
		case OPCODE_lload_2:
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pc++;
			break;
		case OPCODE_lload_3:
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[4].value);
			pc++;
			break;
		case OPCODE_fload_0:
			pushOperandStack( frame->operandStack, frame->localVariableArray[0].value);
			pc++;
			break;
		case OPCODE_fload_1:
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pc++;
			break;
		case OPCODE_fload_2:
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pc++;
			break;
		case OPCODE_fload_3:
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pc++;
			break;
		case OPCODE_dload_0:
			pushOperandStack( frame->operandStack, frame->localVariableArray[0].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pc++;
			break;
		case OPCODE_dload_1:
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pc++;
			break;
		case OPCODE_dload_2:
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pc++;
			break;
		case OPCODE_dload_3:
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pushOperandStack( frame->operandStack, frame->localVariableArray[4].value);
			pc++;
			break;
		case OPCODE_aload_0:
			pushOperandStack( frame->operandStack, frame->localVariableArray[0].value);
			pc++;
			break;
		case OPCODE_aload_1:
			pushOperandStack( frame->operandStack, frame->localVariableArray[1].value);
			pc++;
			break;
		case OPCODE_aload_2:
			pushOperandStack( frame->operandStack, frame->localVariableArray[2].value);
			pc++;
			break;
		case OPCODE_aload_3:
			pushOperandStack( frame->operandStack, frame->localVariableArray[3].value);
			pc++;
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
			pc++;
			index = code[pc];
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index].value = aux_u4;
			pc++;
			break;
		case OPCODE_lstore: //TESTE: ver ordem correta
			pc++;
			index = code[pc];
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index+1].value = aux_u4; //low
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index].value = aux_u4; //high
			pc++;
			break;
		case OPCODE_fstore:
			pc++;
			index = code[pc];
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index].value = aux_u4;
			pc++;
			break;
		case OPCODE_dstore: //TESTE: ver ordem correta
			pc++;
			index = code[pc];
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index].value = aux_u4; //low
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index+1].value = aux_u4; //high
			pc++;
			break;
		case OPCODE_astore:
			pc++;
			index = code[pc];
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[index].value = aux_u4;
			pc++;
			break;
		case OPCODE_istore_0:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[0].value = aux_u4;
			pc++;
			break;
		case OPCODE_istore_1:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			pc++;
			break;
		case OPCODE_istore_2:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			pc++;
			break;
		case OPCODE_istore_3:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			pc++;
			break;
		case OPCODE_lstore_0:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[0].value = aux_u4;
			pc++;
			break;
		case OPCODE_lstore_1:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			pc++;
			break;
		case OPCODE_lstore_2:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			pc++;
			break;
		case OPCODE_lstore_3:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[4].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			pc++;
			break;
		case OPCODE_fstore_0:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[0].value = aux_u4;
			pc++;
			break;
		case OPCODE_fstore_1:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			pc++;
			break;
		case OPCODE_fstore_2:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			pc++;
			break;
		case OPCODE_fstore_3:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			pc++;
			break;
		case OPCODE_dstore_0:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[0].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			pc++;
			break;
		case OPCODE_dstore_1:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			pc++;
			break;
		case OPCODE_dstore_2:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			pc++;
			break;
		case OPCODE_dstore_3:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[4].value = aux_u4;
			pc++;
			break;
		case OPCODE_astore_0:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[0].value = aux_u4;
			pc++;
			break;
		case OPCODE_astore_1:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[1].value = aux_u4;
			pc++;
			break;
		case OPCODE_astore_2:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[2].value = aux_u4;
			pc++;
			break;
		case OPCODE_astore_3:
			aux_u4 = popOperandStack( frame->operandStack);
			frame->localVariableArray[3].value = aux_u4;
			pc++;
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
			popOperandStack( frame->operandStack);
			pc++;
			break;
		case OPCODE_pop2:
			popOperandStack( frame->operandStack);
			popOperandStack( frame->operandStack);
			pc++;
			break;
		case OPCODE_dup:
			aux_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dup_x1:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dup_x2:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux3_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dup2:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dup2_x1:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux3_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dup2_x2:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			aux4_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux4_u4);
			pushOperandStack( frame->operandStack, aux3_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_swap:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_iadd:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 + aux2_u4 );
			pc++;
			break;
		case OPCODE_ladd:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 += aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fadd:
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux2_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f += aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dadd:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux2_d, &aux2_u8, sizeof(u8));
			aux_d += aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_isub:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 - aux2_u4 );
			pc++;
			break;
		case OPCODE_lsub:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 -= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fsub:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f -= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dsub:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8));
			aux_d -= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_imul:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 * aux2_u4 );
			pc++;
			break;
		case OPCODE_lmul:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 *= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fmul:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f *= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dmul:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8));
			aux_d *= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_idiv:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 / aux2_u4 );
			pc++;
			break;
		case OPCODE_ldiv:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 /= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fdiv:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f /= aux2_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_ddiv:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8));
			aux_d /= aux2_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_irem:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 % aux2_u4 );
			pc++;
			break;
		case OPCODE_lrem:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 %= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_frem:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			aux_f = fmodf(aux_f, aux2_f);
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_drem:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4  << 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			memcpy(&aux2_d, &aux2_u8, sizeof(u8));
			aux_d = fmod(aux_d, aux2_d);
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_ineg:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 = -aux_u4;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_lneg:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 = -aux_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fneg:
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux_f = -aux_f;
			memcpy(&aux_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dneg:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_d = -aux_d;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_ishl:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 &= 0x1f; // sao os menores 6 bits = 64, mas o max = 32, logo so seta os menores 5;
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u4 <<= aux_u4;
			pushOperandStack( frame->operandStack, aux2_u4);
			break;
		case OPCODE_lshl:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 &= 0x3f;
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux3_u4<< 32) | aux2_u4;
			aux_u8 <<= aux_u4;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			break;
		case OPCODE_ishr:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 &= 0x1f;
			aux2_u4 = popOperandStack( frame->operandStack);
			for(int i = 0; i < aux_u4; i++) {
				aux2_u4 /= 2;
			}
			pushOperandStack( frame->operandStack, aux2_u4);
			break;
		case OPCODE_lshr:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 &= 0x3f;
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux3_u4<< 32) | aux2_u4;
			for(int i = 0; i < aux_u4; i++) {
				aux_u8 /= 2;
			}
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			break;
		case OPCODE_iushr:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u4 &= 0x1f;
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 >>= aux2_u4;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_lushr:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 &= 0x3f;
			aux2_u4 = popOperandStack( frame->operandStack);
			aux3_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux3_u4<< 32) | aux2_u4;
			aux_u8 >>= aux_u4;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			break;
		case OPCODE_iand:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 & aux2_u4 );
			pc++;
			break;
		case OPCODE_land:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 &= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_ior:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 | aux2_u4 );
			pc++;
			break;
		case OPCODE_lor:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 |= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_ixor:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4 ^ aux2_u4 );
			pc++;
			break;
		case OPCODE_lxor:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u8 ^= aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_iinc:
			pc++;
			aux_u1 = code[pc];
			aux_u4 = frame->localVariableArray[aux_u1].value;
			pc++;
			aux2_u4 = code[pc];
			frame->localVariableArray[aux_u1].value = aux_u4 + aux2_u4;
			pc++;
			break;
		case OPCODE_i2l:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u4 = aux_u4 & 0x80000000;
			aux_u8 = aux_u4;
			if(aux_u4) {
				aux_u8 |= 0xffffffff00000000;
			}
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_i2f:
			aux_u4 = (popOperandStack( frame->operandStack));
			aux_f = (float)aux_u4;
			memcpy(&aux2_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_i2d:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_d = (double)aux_u4;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_l2i:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_l2f:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_f = (float)aux_u8;
			memcpy(&aux3_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux3_u4);
			pc++;
			break;
		case OPCODE_l2d:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_d = (double)aux_u8;
			memcpy(aux2_u8, &aux_d, sizeof(u8));
			aux_u4 = aux2_u8>>32;//high
			aux2_u4 = aux2_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2i:
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux2_u4 = (u4) aux_f;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2l:
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux_u8 = (u8) aux_f;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2d:
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			aux_d = (double) aux_f;
			memcpy(&aux_u8, &aux_d, sizeof(u8));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_d2i:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_u4 = (u4) aux_d;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_d2l:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_u8 = (u8) aux_d;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_d2f:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4  << 32) | aux_u4;
			memcpy(&aux_d, &aux_u8, sizeof(u8));
			aux_f = (float) aux_d;
			memcpy(&aux3_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux3_u4);
			pc++;
			break;
		case OPCODE_i2b:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u1 = (u1)aux_u4;
			aux2_u4 = aux_u1;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_i2c:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u2 = (u2)aux_u4;
			aux2_u4 = aux_u2;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_i2s:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u2 = (u2)aux_u4;
			aux2_u4 = aux_u2;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_lcmp:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			if(aux_u8 == aux2_u8) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_u8 > aux2_u8) {
				pushOperandStack( frame->operandStack, 1);
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_fcmpl:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			memcpy(&aux_f, &aux_u4, sizeof(u4));
			memcpy(&aux2_f, &aux2_u4, sizeof(u4));
			if(aux_f == aux2_f) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_f > aux2_f) {
				pushOperandStack( frame->operandStack, 1);;
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_fcmpg:
			break;
		case OPCODE_dcmpl:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux2_d = (double) aux2_u8;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_d = (double) aux_u8;
			if(aux_d == aux2_d) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_d > aux2_d) {
				pushOperandStack( frame->operandStack, 1);
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_dcmpg:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = (aux2_u4<< 32) | aux_u4;
			aux2_d = (double) aux2_u8;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = (aux2_u4<< 32) | aux_u4;
			aux_d = (double) aux_u8;
			if(aux_d == aux2_d) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_d > aux2_d) {
				pushOperandStack( frame->operandStack, 1);
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_ifeq:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4==0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_ifne:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4!=0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_iflt:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4<0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_ifge:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4>=0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_ifgt:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4>0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_ifle:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			if(aux_u4<=0){
				aux2_u4 = (u4) aux_u2;
				pc +=aux2_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmpeq:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4==aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmpne:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4!=aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmplt:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4<aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmpge:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4>=aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmpgt:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4>aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_icmple:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4<=aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_acmpeq:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4==aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_if_acmpne:
			aux_u2 = (code[pc] << 8) | code[pc+1];
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			if(aux_u4!=aux2_u4){
				aux3_u4 = (u4) aux_u2;
				pc +=aux3_u4;
			}
			else{
				pc+=3;
			}
			break;
		case OPCODE_goto:
		    branchoffset = (code[pc] << 8) | code[pc+1]; ///Calcula o endereco para o qual o GOTO ira mandar o pc
		    memcpy(aux_u4, &branchoffset, sizeof(u4)); /// Grava o valor do branchoffset em uma variavel de 32 bits
		    pc += aux_u4; ///Atribui o valor calculado ao pc para que haja o deslocamento solicitado
			pc++;
			break;
		case OPCODE_jsr:
		    pushOperandStack(frame->operandStack, pc+1); ///Salva na pilha o valor de retorno da proxima instrucao depois do JSR
		    branchoffset = (code[pc] << 8) | code[pc+1]; ///Calcula o endereco para onde deve ser deslocado o pc
		    memcpy(aux_u4, &branchoffset, sizeof(u4)); /// Grava o valor do branchoffset em uma variavel de 32 bits
		    pc += aux_u4; ///Atribui o valor calculado ao pc para que haja o deslocamento solicitado
			pc++;
			break;
		case OPCODE_ret: ///Return from subroutine
		    pc++;
		    if(fWide == 1){ ///Checa se a flag da instrucao wide esta setada
                index = (code[pc] << 8) | code[pc+1]; ///Se sim, o index é composto por dois bytes
                pc++;
		    }
		    else{
                index = code[pc]; ///Se nao, é por apenas 1 byte
                if(index < 0 || index > 255)
                    printf("Erro\n"); ///Deve-se trocar esse printf por uma excessao porque o index tem que estar dentro da faixa 0-255
		    }
		    pc = frame->localVariableArray[index].value; ///PC = o valor dentro da posicao buscada no vetor de variaveis globais como endereco de retorno
			pc++;
			break;
		case OPCODE_tableswitch: ///Access jump table by index and jump
            //pc++;
            instr_tableSwitch(frame, pc, code);
            pc++;
			break;
		case OPCODE_lookupswitch: /// Access jump table by key match and jump
            instr_lookUpSwitch(frame, pc, code);
            pc++;
			break;
		case OPCODE_ireturn: /// Return int from method
		    returnAddress = popOperandStack(frame->operandStack);
		    printf("AINDA NAO IMPLEMENTADO\n");
            ///tenho que dar free na pilha de operandos inteira
            ///altera a variavel global de retorno
            ///altera o tipo da variavel global de retorno
            ///Arrumar um jeito de colocar na pilha de operandos do proximo frame
			break;
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
			printf("instrucao nao implementada\n");
			break;
		case OPCODE_checkcast:
			printf("instrucao nao implementada\n");
			break;
		case OPCODE_instanceof:
			printf("instrucao nao implementada\n");
			break;
		case OPCODE_monitorenter:
			printf("instrucao nao implementada\n");
			break;
		case OPCODE_monitorexit:
			printf("instrucao nao implementada\n");
			break;
		case OPCODE_wide:
            fWide = 1;
            pc++;
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
			printf("instrucao nao implementada\n");
			break;
	}

}
