#define CLASSMANAGER_SERV
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "lib/instruction.h"
//#include "lib/macros.h"

///funcao de debug, so apagar depois

void printStack(structOperandStack **operandStackTop){
    structOperandStack *aux;
    aux = *operandStackTop;
    printf("Operand Stack: ");
    if(aux == NULL)
        printf("null\n");
    while(aux != NULL){
        printf("%d -> ", aux->value);
        aux = aux->next;
    }
    printf("\n");
}

///Pega o field index base na posicao do fieldinfo no class file
u4 getFieldIndex(field_info * fields, char * fName, u2 fields_count, cp_info * constant_pool ){
    char * auxName;
    for(int i = 0; i < fields_count; i++){
        auxName = constant_pool[fields[i].name_index].UTF8.bytes;
        if(strstr(fName, auxName) != NULL){
            return i;
        }
    }
    return -1;
}

///resolve o nome e tipo do field e preenche o valor do field estatico
void instr_putstatic(Frame * frame, u4 pc, u1 * code){//FIELDREF
	u2 index, ntIndex;
	u4 cIndex, fIndex, aux_u4, aux2_u4;
	char *cName, *fName, *fType;

	index = (code[pc+1] << 8) | code[pc+2];
	cIndex = frame->constant_pool[index].Fieldref.class_index;
	ntIndex = frame->constant_pool[index].Fieldref.name_and_type_index;
	fName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_index].UTF8.bytes;
	fType = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

	fIndex = getFieldIndex(frame->handler->fields, fName, frame->handler->classRef->fields_count, frame->constant_pool);
	if(fIndex<0){
        return;
	}

	if((strstr(fType, "[") != NULL)||(strstr(fType, "L") != NULL)){
        frame->handler->classRef->field_value[fIndex].U4.value = popOperandStack( frame->operandStack);
    } else if((strstr(fType, "J") != NULL)||(strstr(fType, "D") != NULL)){
        frame->handler->classRef->field_value[fIndex].U8.low = popOperandStack( frame->operandStack);
        frame->handler->classRef->field_value[fIndex].U8.high = popOperandStack( frame->operandStack);
    }else{
        frame->handler->classRef->field_value[fIndex].U4.value = popOperandStack( frame->operandStack);
    }

}

///resolve o nome e tipo do field e pega o valor do field estatico
void instr_getstatic(Frame * frame, u4 pc, u1 * code){
	u2 index, ntIndex;
	u4 cIndex, fIndex, aux_u4, aux2_u4;
	char *cName, *fName, *fType;

	index = (code[pc+1] << 8) | code[pc+2];
	cIndex = frame->constant_pool[index].Fieldref.class_index;
	ntIndex = frame->constant_pool[index].Fieldref.name_and_type_index;
	fName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_index].UTF8.bytes;
	fType = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

	fIndex = getFieldIndex(frame->handler->fields, fName, frame->handler->classRef->fields_count, frame->constant_pool);
	if(fIndex<0){
        return;
	}

	if((strstr(fType, "[") != NULL)||(strstr(fType, "L") != NULL)){
        pushOperandStack( frame->operandStack, frame->handler->classRef->field_value[fIndex].U4.value);
//        printf("Referencia\n");
    } else if((strstr(fType, "J") != NULL)||(strstr(fType, "D") != NULL)){
        pushOperandStack( frame->operandStack, frame->handler->classRef->field_value[fIndex].U8.high);
        pushOperandStack( frame->operandStack, frame->handler->classRef->field_value[fIndex].U8.low);
//        printf("double ou long\n");
    }else{
//        printf("u4\n");
        pushOperandStack( frame->operandStack, frame->handler->classRef->field_value[fIndex].U4.value);
    }

}

void instr_getfield(Frame * frame, u4 pc, u1 * code){
	u2 index, ntIndex;
	u4 cIndex, fIndex, aux_u4, aux2_u4;
	char *cName, *fName, *fType;
	ClassHandler * aux_ch;

	index = (code[pc+1] << 8) | code[pc+2];
	cIndex = frame->constant_pool[index].Fieldref.class_index;
	ntIndex = frame->constant_pool[index].Fieldref.name_and_type_index;
	fName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_index].UTF8.bytes;
	fType = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

    aux_ch = (ClassHandler*)popOperandStack(frame->operandStack);

	fIndex = getFieldIndex(aux_ch->fields, fName, aux_ch->classRef->fields_count, frame->constant_pool);
	if(fIndex<0){
        return;
	}

	if((strstr(fType, "[") != NULL)||(strstr(fType, "L") != NULL)){
        pushOperandStack( frame->operandStack, aux_ch->field_value[fIndex].U4.value);
//        printf("Referencia\n");
    } else if((strstr(fType, "J") != NULL)||(strstr(fType, "D") != NULL)){
        pushOperandStack( frame->operandStack, aux_ch->field_value[fIndex].U8.high);
        pushOperandStack( frame->operandStack, aux_ch->field_value[fIndex].U8.low);
//        printf("double ou long\n");
    }else{
//        printf("u4\n");
        pushOperandStack( frame->operandStack, aux_ch->field_value[fIndex].U4.value);
    }

}

void instr_putfield(Frame * frame, u4 pc, u1 * code){//FIELDREF
	u2 index, ntIndex;
	u4 cIndex, fIndex, aux_u4, aux2_u4, flag;
	char *cName, *fName, *fType;
    ClassHandler * aux_ch;

    flag = 0;
	index = (code[pc+1] << 8) | code[pc+2];
	cIndex = frame->constant_pool[index].Fieldref.class_index;
	ntIndex = frame->constant_pool[index].Fieldref.name_and_type_index;
	fName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_index].UTF8.bytes;
	fType = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;



	if((strstr(fType, "[") != NULL)||(strstr(fType, "L") != NULL)){
        aux_u4 = popOperandStack( frame->operandStack);
    } else if((strstr(fType, "J") != NULL)||(strstr(fType, "D") != NULL)){
        aux_u4 = popOperandStack( frame->operandStack);
        aux2_u4 = popOperandStack( frame->operandStack);
        flag = 1;
    }else{
        aux_u4 = popOperandStack( frame->operandStack);
    }

    aux_ch = (ClassHandler*)popOperandStack(frame->operandStack);
	fIndex = getFieldIndex(aux_ch->fields, fName, aux_ch->classRef->fields_count, frame->constant_pool);
	if(fIndex<0){
        return;
	}

    if(flag==1){
        aux_ch->field_value[fIndex].U8.low = aux_u4;
        aux_ch->field_value[fIndex].U8.high = aux2_u4;
    }
    else{
        aux_ch->field_value[fIndex].U4.value = aux_u4;
    }
}

///Printa o que estiver na pilha de acordo com o tipo
void instr_invokeVirtual(Frame * frame, u4 pc, u1 fWide, u1 * code){
	u2 index, ntIndex;
	u4 cIndex, fIndex, aux_u4, aux2_u4;
	u8 aux_u8, aux2_u8;
	char *cName, *mName, *mDesc;
	float aux_f;
	double aux_d;

	pc++;
	index = (code[pc] << 8) | code[pc+1];
	cIndex = frame->constant_pool[index].Methodref.class_index;
	ntIndex = frame->constant_pool[index].Methodref.name_and_type_index;
	cName =  frame->constant_pool[frame->constant_pool[cIndex].Class.name_index].UTF8.bytes;
	mName = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.name_index].UTF8.bytes;
	mDesc = frame->constant_pool[frame->constant_pool[ntIndex].NameAndType.descriptor_index].UTF8.bytes;

	if((strcmp(cName, "java/io/PrintStream") == 0) &&((strcmp(mName,"print") == 0) ||(strcmp(mName,"println") == 0))){
		if(strstr(mDesc, "J") != NULL){//long
            aux_u4  = popOperandStack( frame->operandStack);
		    aux2_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
            printf("%lld", aux_u8);
		} else if(strstr(mDesc, "D") != NULL) {//double
		    aux2_u4  = popOperandStack( frame->operandStack);//low
		    aux_u4 = popOperandStack( frame->operandStack);//high
		    aux_u8 = aux2_u4;
		    aux_u8 <<=32;
		    aux2_u8 = (unsigned) aux_u4;
			aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
		    printf("%f", aux_d);
        } else if(strstr(mDesc, "Z") != NULL) {//boolean
            if(popOperandStack( frame->operandStack)) {
				printf("True");
			} else {
				printf("False");
			}
		} else if(strstr(mDesc, "C") != NULL) {//char
            aux_u4  = popOperandStack( frame->operandStack);
            printf("%c",aux_u4);
            //if(strstr(mDesc, "[C") != NULL) {//array char
		} else if(strstr(mDesc, "I") != NULL) {//int
		    aux_u4  = popOperandStack( frame->operandStack);
		    if(strstr(mDesc, "[I") != NULL)
                printf("**%d**",(u4 *) aux_u4);
            else{
                printf("%d", aux_u4);
            }
		} else if(strstr(mDesc, "F") != NULL) {//float
		    aux_u4  = popOperandStack( frame->operandStack);
            aux_f = *((float*)&(aux_u4));
            printf("%f",aux_f);
		} else if(strstr(mDesc, "Ljava/lang/String") != NULL) {//string
		    aux_u4 = popOperandStack( frame->operandStack);
		    printf("%s", (char *)aux_u4);
		}
		if(strstr(mName, "println") != NULL) {
			printf("\n");
		}
	}
}


int16_t printBin(int16_t var, int dim){
    float value=0;
    for(int i=0; i<dim; i++){
        printf("%d ", (var >> (dim - i-1)) & 1);
        if(((var >> (dim - i-1)) & 1) == 1){
            if(dim- i -1 == 15){
                value -= pow(2,dim-i-1);
            }
            else{
                value += (pow(2,(dim-i-1)));
            }
        }
    }
    printf("valor = %.0f\n", value);
    return ((int16_t)value);
}

void doInstructionShift(Frame **cur_frame/*, u1 curOPCODE*/, u4 *curPC, StructFrameStack *frameStackTop, u1 *code, u1 fWide){
    u1 aux_u1, aux2_u1;
	u2 aux_u2, index;
	int16_t branchoffset;
	u4 aux_u4, aux2_u4, aux3_u4, aux4_u4, returnValue, returnValue2, opcodeAddress;
	u8 aux_u8, aux2_u8;
	float aux_f, aux2_f;
	double aux_d, aux2_d;

     switch(code[*curPC]){
        case OPCODE_ifeq:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4==0){
				aux2_u4 = (u4) branchoffset;
				curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_ifne:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4!=0){
				aux2_u4 = (u4) branchoffset;
				*curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_iflt:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4<0){
				aux2_u4 = (u4) branchoffset;
				*curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_ifge:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4>=0){
				aux2_u4 = (u4) branchoffset;
				*curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_ifgt:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4>0){
				aux2_u4 = (u4) branchoffset;
				*curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_ifle:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4<=0){
				aux2_u4 = (u4) branchoffset;
				*curPC +=aux2_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_icmpeq:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4==aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_icmpne:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4!=aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_icmplt:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4<aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
//			printf("var1:%d var2:%d PC:%d soma: %d\n", aux_u4, aux2_u4, *curPC, aux3_u4);
			break;
		case OPCODE_if_icmpge:
		    //(*curPC)++;
			branchoffset = (code[*curPC+1] << 8) | code[(*curPC)+2];
			//printBin((code[*curPC+1] << 8) | code[(*curPC)+2], 16);
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4>=aux2_u4){
				aux3_u4 = (u4) branchoffset;
				(*curPC) += aux3_u4;
			}
			else{
				(*curPC) += 3;
			}
			//printf("if_icmpge curPC: %d\n", *curPC);
			//getchar();
			break;
		case OPCODE_if_icmpgt:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4>aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_icmple:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4<=aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_acmpeq:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4==aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_if_acmpne:
			branchoffset = (code[*curPC+1] << 8) | code[*curPC+2];
			aux2_u4 = popOperandStack( (*cur_frame)->operandStack);
			aux_u4 = popOperandStack( (*cur_frame)->operandStack);
			if(aux_u4!=aux2_u4){
				aux3_u4 = (u4) branchoffset;
				*curPC +=aux3_u4;
			}
			else{
				*curPC+=3;
			}
			break;
		case OPCODE_goto:
		    opcodeAddress = *curPC;
		    (*curPC)++;
		    branchoffset = (code[opcodeAddress+1] << 8) | code[opcodeAddress+2];  ///Calcula o endereco para o qual o GOTO ira mandar o pc
		    aux_u4 = ((u4)branchoffset);
		    *curPC = opcodeAddress + aux_u4; ///Atribui o valor calculado ao pc para que haja o deslocamento solicitado
		    //printf("curPC: %d\n", *curPC);
		    //getchar();
			break;
		case OPCODE_jsr:
		    //(*curPC)++;
		    pushOperandStack((*cur_frame)->operandStack, (*curPC)+3); ///Salva na pilha o valor de retorno da proxima instrucao depois do JSR
		    branchoffset = (code[*curPC+1] << 8) | code[*curPC+2]; ///Calcula o endereco para onde deve ser deslocado o pc
		    memcpy(aux_u4, &branchoffset, sizeof(u4)); /// Grava o valor do branchoffset em uma variavel de 32 bits
		    (*curPC) += aux_u4; ///Atribui o valor calculado ao pc para que haja o deslocamento solicitado
			break;
		case OPCODE_ret: ///Return from subroutine
		    (*curPC)++;
		    if(fWide == 1){ ///Checa se a flag da instrucao wide esta setada
                index = (code[*curPC] << 8) | code[*curPC+1]; ///Se sim, o index é composto por dois bytes
                (*curPC)++;
		    }
		    else{
                index = code[*curPC]; ///Se nao, é por apenas 1 byte
                if(index > 255)
                    printf("Erro\n"); ///Deve-se trocar esse printf por uma excessao porque o index tem que estar dentro da faixa 0-255
		    }
		    *curPC = (*cur_frame)->localVariableArray[index].value; ///PC = o valor dentro da posicao buscada no vetor de variaveis globais como endereco de retorno
			break;
		case OPCODE_tableswitch: ///Access jump table by index and jump
            instr_tableSwitch((*cur_frame), curPC, code);
			break;
		case OPCODE_lookupswitch: /// Access jump table by key match and jump
            instr_lookUpSwitch((*cur_frame), curPC, code);
			break;
		case OPCODE_ireturn: /// Return int from method
		    returnValue = popOperandStack((*cur_frame)->operandStack); /// Desempilha o valor de retorno
		    *curPC = (*cur_frame)->returnPC; /// Carrega o valor do PC do metodo chamador
		    while((*cur_frame)->operandStack->next != NULL){ ///Loop para esvaziar(desempilhar) a pilha
                popOperandStack((*cur_frame)->operandStack);
		    }
		    Frame * tmp_frame = (Frame *)malloc(sizeof(Frame));
		    tmp_frame = popFrameStack(frameStackTop); ///CArrega o frame chamador do metodo retornado na memoria
		    pushOperandStack(tmp_frame->operandStack, returnValue); ///Empilha o valor de retorno na pilha
		    (*cur_frame) = tmp_frame;
			break;

		case OPCODE_lreturn: /// Return long from method
		    returnValue = popOperandStack((*cur_frame)->operandStack); /// Desempilha o primeiro valor de retorno
		    returnValue2 = popOperandStack((*cur_frame)->operandStack); /// Desempilha o valor de retorno
		    *curPC = (*cur_frame)->returnPC; ///Carrega o valor do PC do metodo chamador
		    while((*cur_frame)->operandStack->next != NULL){    ///Esvazia a pilha de operando do frame que está sendo finalizado
                popOperandStack((*cur_frame)->operandStack);
		    }
		    (*cur_frame) = popFrameStack(frameStackTop);    ///Carrega o frame chamador do metodo retornado na memoria
		    pushOperandStack((*cur_frame)->operandStack, returnValue2);///Empilha o valor de retorno na pilha (Long)
		    pushOperandStack((*cur_frame)->operandStack, returnValue);
			break;

		case OPCODE_freturn: /// Return float from method
		    returnValue = popOperandStack((*cur_frame)->operandStack); /// Desempilha o valor de retorno
		    *curPC = (*cur_frame)->returnPC; /// Carrega o valor do PC do metodo chamador
		    while((*cur_frame)->operandStack->next != NULL){ ///Loop para esvaziar(desempilhar) a pilha
                popOperandStack((*cur_frame)->operandStack);
		    }
		    (*cur_frame) = popFrameStack(frameStackTop); ///CArrega o frame chamador do metodo retornado na memoria
            pushOperandStack((*cur_frame)->operandStack, returnValue); ///Empilha o valor de retorno na pilha
			break;

		case OPCODE_dreturn: ///Return double from method
		    returnValue = popOperandStack((*cur_frame)->operandStack); /// Desempilha o primeiro valor de retorno
		    returnValue2 = popOperandStack((*cur_frame)->operandStack); /// Desempilha o valor de retorno
		    *curPC = (*cur_frame)->returnPC; ///Carrega o valor do PC do metodo chamador
		    while((*cur_frame)->operandStack->next != NULL){    ///Esvazia a pilha de operando do frame que está sendo finalizado
                popOperandStack((*cur_frame)->operandStack);
		    }
		    (*cur_frame) = popFrameStack(frameStackTop);    ///Carrega o frame chamador do metodo retornado na memoria
		    pushOperandStack((*cur_frame)->operandStack, returnValue2);///Empilha o valor de retorno na pilha (Long)
		    pushOperandStack((*cur_frame)->operandStack, returnValue);
			break;

		case OPCODE_areturn:
		    returnValue = popOperandStack((*cur_frame)->operandStack); /// Desempilha o valor de retorno
		    *curPC = (*cur_frame)->returnPC; /// Carrega o valor do PC do metodo chamador
		    while((*cur_frame)->operandStack->next != NULL){ ///Loop para esvaziar(desempilhar) a pilha
                popOperandStack((*cur_frame)->operandStack);
		    }
		    (*cur_frame) = popFrameStack(frameStackTop); ///CArrega o frame chamador do metodo retornado na memoria
            pushOperandStack((*cur_frame)->operandStack, returnValue); ///Empilha o valor de retorno na pilha
			break;

		case OPCODE_return:
//		    printf("RETURN\n");
		    while((*cur_frame)->operandStack->next != NULL){
                popOperandStack((*cur_frame)->operandStack);
//                printf("loop free operand stack\n");
		    }
		    if((*cur_frame)->returnPC != NOT_RETURN){
                //printf("entrou1\n");
                *curPC = (*cur_frame)->returnPC;
                (*cur_frame) = popFrameStack(frameStackTop);
		    }
		    else {
                //printf("entrou2\n");
                *curPC = NOT_RETURN; ///significa que é o return da main
		    }
			break;
    }
}



void doInstruction(Frame * frame, u4 pc, u1 fWide, u1 * code ){
	u1 aux_u1, aux2_u1;
	int8_t aux_i8;
	int16_t aux_i16;
	u2 aux_u2, index, branchoffset;
	u4 aux_u4, aux2_u4, aux3_u4, aux4_u4, returnAddress;
	u8 aux_u8, aux2_u8;
	float aux_f, aux2_f;
	double aux_d, aux2_d;

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
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_fconst_1:
			aux_f = 1.0;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			break;
		case OPCODE_fconst_2:
			aux_f = 2.0;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dconst_0:
			aux_d = 0.0;
			aux_u8 = *((u8*)&(aux_d));
			aux_u4 = aux_u8>>32;
			pushOperandStack( frame->operandStack, aux_u4);
			aux_u4 = aux_u8;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dconst_1:
			aux_d= 1.0;
			aux_u8 = *((u8*)&(aux_d));
			aux_u4 = aux_u8>>32;
			pushOperandStack( frame->operandStack, aux_u4);
			aux_u4 = aux_u8;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_bipush:
			pc++;

			aux_i8 = code[pc];
			aux_u4 = aux_i8;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_sipush:
			aux_i16 = (code[pc+1] << 8) | code[pc+2];
			aux_u4 = aux_i16;
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
				case(CONSTANT_String):
					aux_u4 = frame->constant_pool[index].String.string_index;
					pushOperandStack( frame->operandStack, frame->constant_pool[aux_u4].UTF8.bytes);
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
				case(CONSTANT_String):
					aux_u4 = frame->constant_pool[index].String.string_index;
					pushOperandStack( frame->operandStack, frame->constant_pool[aux_u4].UTF8.bytes);
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u8 += aux2_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fadd:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_f = *((float*)&(aux2_u4));
			aux_f += aux2_f;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dadd:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux_d += aux2_d;
			aux_u8 = *((u8*)&(aux_d));
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
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
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
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
			aux_f -= aux2_f;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dsub:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_d -= aux2_d;
//			printf("***%f\n", aux2_d);
//			printf("---%f\n", aux_d);
			aux_u8 = *((u8*)&(aux_d));
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
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
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
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
			aux_f *= aux2_f;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dmul:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_d *= aux2_d;
			aux_u8 = *((u8*)&(aux_d));
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
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
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
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
			aux_f /= aux2_f;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_ddiv:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_d /= aux2_d;
			aux_u8 = *((u8*)&(aux_d));
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
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
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
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
			aux_f = fmodf(aux_f, aux2_f);
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_drem:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_d = fmod(aux_d, aux2_d);
			aux_u8 = *((u8*)&(aux_d));
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_u8 = -aux_u8;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_fneg:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux_f = -aux_f;
			aux_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_dneg:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_d = -aux_d;
			aux_u8 = *((u8*)&(aux_d));
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
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
			aux_u8 = aux_u4;
			if(aux_u4 & 0x80000000) {
				aux_u8 = 0xffffffff00000000|aux_u8;
			}
			else
                aux_u8 = 0x000000000000000|aux_u8;
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
			aux_u8 = *((u8*)&(aux_d));
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
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
			aux_f = aux_u8;
			aux3_u4= *((u4*)&(aux_f));
			pushOperandStack( frame->operandStack, aux3_u4);
			pc++;
			break;
		case OPCODE_l2d:
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = (double)aux_u8;
			aux_u8 = *((u8*)&(aux_d));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2i:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux2_u4 = (u4) aux_f;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2l:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux_u8 = (u8) aux_f;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_f2d:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux_d = (double) aux_f;
			aux_u8 = *((u8*)&(aux_d));
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_d2i:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_u4 = aux_d;
			pushOperandStack( frame->operandStack, aux_u4);
			pc++;
			break;
		case OPCODE_d2l:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_u8 = (u8) aux_d;
			aux_u4 = aux_u8>>32;//high
			aux2_u4 = aux_u8 & 0xffffffff;//low
			pushOperandStack( frame->operandStack, aux_u4);
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_d2f:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			aux_f = (float) aux_d;
			memcpy(&aux3_u4, &aux_f, sizeof(u4));
			pushOperandStack( frame->operandStack, aux3_u4);
			pc++;
			break;
		case OPCODE_i2b:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_i8 = (u1)aux_u4;
			aux2_u4 = aux_i8;
			pushOperandStack( frame->operandStack, aux2_u4);
			pc++;
			break;
		case OPCODE_i2c:
			aux_u4 = popOperandStack( frame->operandStack);
			aux_i16 = (u2)aux_u4;
			aux2_u4 = aux_i16;
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
			aux2_u8 = aux2_u4;
			aux2_u8 = (aux2_u8<<32) | aux_u4;
			aux_u4 = popOperandStack( frame->operandStack);
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u8 = aux2_u4;
			aux_u8 = (aux_u8<<32) | aux_u4;
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
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
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
		    aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
			aux_f = *((float*)&(aux_u4));
			aux2_f = *((float*)&(aux2_u4));
			if(aux_f == aux2_f) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_f > aux2_f) {
				pushOperandStack( frame->operandStack, 1);;
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_dcmpl:
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
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
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux2_d = *((double*)&(aux_u8));
			aux2_u4 = popOperandStack( frame->operandStack);
			aux_u4 = popOperandStack( frame->operandStack);
            aux_u8 = aux2_u4;
            aux_u8 <<=32;
            aux2_u8 = (unsigned) aux_u4;
            aux_u8 = aux_u8 |aux2_u8;
			aux_d = *((double*)&(aux_u8));
			if(aux_d == aux2_d) {
				pushOperandStack( frame->operandStack, 0);
			} else if(aux_d > aux2_d) {
				pushOperandStack( frame->operandStack, 1);
			} else {
				pushOperandStack( frame->operandStack, -1);
			}
			pc++;
			break;
		case OPCODE_getstatic:
		    instr_getstatic(frame, pc, code);
			break;
		case OPCODE_putstatic:
		    instr_putstatic(frame, pc, code);
			break;
		case OPCODE_getfield:
		    instr_getfield(frame, pc, code);
			break;
		case OPCODE_putfield:
		    instr_putfield(frame, pc, code);
			break;
		case OPCODE_new:
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
		case OPCODE_breakpoint:
			printf("instrucao nao implementada\n");
			break;
	}

}

void instr_tableSwitch(Frame *frame, u4 *curPC, u1 * code){
    u1 byte1, byte2, byte3, byte4;
    u4 highTableS, lowTableS, defaultTableS, valTableS, opcodeAddress, targetAddress, *tableSwitch;

    opcodeAddress = *curPC;
    while((*curPC + 1)%4 != 0){ ///Loop para o preenchimento do padding <0-3 bytes>
        (*curPC)++;
    }
    (*curPC)++;

    byte1 = code[(*curPC)++];
    byte2 = code[(*curPC)++];
    byte3 = code[(*curPC)++];
    byte4 = code[(*curPC)++];
    defaultTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[(*curPC)++];
    byte2 = code[(*curPC)++];
    byte3 = code[(*curPC)++];
    byte4 = code[(*curPC)++];
    lowTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[(*curPC)++];
    byte2 = code[(*curPC)++];
    byte3 = code[(*curPC)++];
    byte4 = code[(*curPC)++];
    highTableS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    valTableS = popOperandStack(frame->operandStack);
    tableSwitch = (u4*)malloc((highTableS-lowTableS+1)*sizeof(u4));///Aloca memória para a tabela(vetor) do switch
    for(int i=0; i < highTableS-lowTableS+1; i++){ /// Preenche a tabela (vetor) com os offsets de cada case
        byte1 = code[(*curPC)++];
        byte2 = code[(*curPC)++];
        byte3 = code[(*curPC)++];
        byte4 = code[(*curPC)++];
        tableSwitch[i] = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    }

    if(valTableS < lowTableS || valTableS > highTableS){
        targetAddress = opcodeAddress + defaultTableS;
    }
    else{
        targetAddress = opcodeAddress + tableSwitch[valTableS-lowTableS];
    }
    *curPC = targetAddress; /// PC e mandado para o offset selecionado
}

void instr_lookUpSwitch(Frame *frame, u4 *curPC, u1 * code){
    u1 byte1, byte2, byte3, byte4, found;
    u4 *lookupSwitch, opcodeAddress, defaultLS, targetAddress, npair, i, key;
    opcodeAddress = (*curPC);
    while(((*curPC) + 1)%4 != 0){
        (*curPC)++;
    }
    (*curPC)++;

    byte1 = code[(*curPC)++];
    byte2 = code[(*curPC)++];
    byte3 = code[(*curPC)++];
    byte4 = code[(*curPC)++];
    defaultLS = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    byte1 = code[(*curPC)++];
    byte2 = code[(*curPC)++];
    byte3 = code[(*curPC)++];
    byte4 = code[(*curPC)++];
    npair = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; ///Quantidade de itens a serem percorridos

    lookupSwitch = (u4*)malloc(npair*2*sizeof(u4)); /// armazena em um vetor o dobro do espaço de variaveis do switch para armazenar os valores dentro deles

    for(int i=0; i< npair*2; i += 2){ ///Loop anda de dois em dois para pegar dois valores de uma vez
        byte1 = code[(*curPC)++];
        byte2 = code[(*curPC)++];
        byte3 = code[(*curPC)++];
        byte4 = code[(*curPC)++];
        lookupSwitch[i] = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4; /// match

        byte1 = code[(*curPC)++];
        byte2 = code[(*curPC)++];
        byte3 = code[(*curPC)++];
        byte4 = code[(*curPC)++];
        lookupSwitch[i+1] = ((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4); /// offset (key)

    }

    key = popOperandStack(frame->operandStack); ///recupera o valor do topo da pilha
    i=0;
    while( found != 1 && i < ((npair*2)-1)){///o loop para quando ele chega no limite do array - 1 ou quando encontra o match
        if(key == lookupSwitch[i]){
            found = 1; /// Flag para quando o match é encontrado
        }
        else
        i+=2; ///incrementa de dois em dois para buscar entre os match's
    }
    if(found == 0){/// caso nao encontre
        targetAddress = defaultLS + opcodeAddress;
    }
    else{///caso encontre
        targetAddress = lookupSwitch[i+1] + opcodeAddress; ///pega o offset seguido do match selecionado
    }

    *curPC = targetAddress;
}


void  doInvokestatic(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 *curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction){
     ///Carrega as informacoes da Classe e Metodo
     u2 method_ref = code[*curPC+1];
     method_ref = method_ref << BYTE_SIZE | code[*curPC+2];
     u2 class_index = cur_frame->constant_pool[method_ref].Methodref.class_index;
     u2 class_name_index = cur_frame->constant_pool[class_index].Class.name_index;
     u2 name_and_type_index = cur_frame->constant_pool[method_ref].Methodref.name_and_type_index;
     u2 name_index = cur_frame->constant_pool[name_and_type_index].NameAndType.name_index;
     u2 descriptor_index = cur_frame->constant_pool[name_and_type_index].NameAndType.descriptor_index;
     u1 * method_descriptor = cur_frame->constant_pool[descriptor_index].UTF8.bytes; ///Descricoa do metodo
     u1 * method_name = cur_frame->constant_pool[name_index].UTF8.bytes; ///Nome do metodo
     u1 * class_name = cur_frame->constant_pool[class_name_index].UTF8.bytes; ///Nome da classe

//     printf("method_descriptor: %s\n",method_descriptor);
//     printf("method_name: %s\n",method_name);
//     printf("class_name: %s\n",class_name);

    ///Caso seja necessario, carrega nova classe no HEAP
    Frame * tmp_frame;// = (Frame *)malloc(sizeof(Frame));
    u4 method_index;
    u2 this_class_index = cur_frame->handler->classRef->this_class;
    u2 this_class_name_index = cur_frame->constant_pool[this_class_index].Class.name_index;
    u1 this_class_name;

    if(this_class_name_index != class_name_index){
         this_class_name = cur_frame->constant_pool[this_class_name_index].UTF8.bytes; ///Nome da classe dona do Frame corrente.
        ///Caso o nome da classe dona do novo metodo seja diferente da classe corrente.
        u4 new_class_index_heap = loadNewClass(class_file,numberOfClassesHeap,class_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);
        ///Procura pelo indice do metodo no method_info da classe
        method_index = seekNewMethodInClassHandler(handler+new_class_index_heap, method_name, method_descriptor); ///Pega o indice do metodo no method_info da classe.
        createNewFrame(handler+new_class_index_heap,method_index, *curPC+3,frameStackTop); ///Cria novo Frame para o novo metodo
    }else{
        ///Caso o metodo chamado seja da mesma classe do metodo chamador
        ///Procura pelo indice do metodo no method_info da classe
        method_index = seekNewMethodInFrameClass(cur_frame, method_name, method_descriptor); ///Pega o indice do metodo no method_info da classe.
        u4 class_index_heap = loadNewClass(class_file,numberOfClassesHeap,handler->classRef->class_full_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);
        if(handler->classRef->methods[method_index].access_flags & ACC_NATIVE){
            *curPC = *curPC + 3;
            return;
        }
        createNewFrame(handler+class_index_heap,method_index, *curPC+3,frameStackTop); ///Cria novo Frame para o novo metodo
    }

    ///Cria o Novo Frame
    tmp_frame = (Frame *)malloc(sizeof(Frame));
        tmp_frame = popFrameStack(frameStackTop);///Retira o novo frame da pilha


    ///Coloca os parametros da pilha de operandos do Frame corrente.
     u2 method_descriptor_len = strlen(method_descriptor);
     int num_parans = 0;
     int localVariableArray_index = 0;
     u4 value_low,value_high,value;

     for(int i = method_descriptor_len-3; i >= 0; i--){
        if(method_descriptor[i] != '(' && method_descriptor[i] != ')' && method_descriptor[i] != 'V' && method_descriptor[i] != '['){
            if(method_descriptor[i] == 'J' || method_descriptor[i] == 'D'){
                num_parans +=2;
            }else{
               num_parans++;
            }
        }
     }

     localVariableArray_index = num_parans-1;
     num_parans = 0;
     for(int i = method_descriptor_len-3; i > 0; i--){
        if(method_descriptor[i] == '(')
            break;
        switch(method_descriptor[i]){
        case 'D': ///Eh um double
            if(method_descriptor[i-1] != '['){
                value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
                value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
                //tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                    //printf("[D] value_high: %d\t value_low: %d\n",value_high,value_low);
                //pushOperandStack(tmp_frame->operandStack,value_high);///Empilha  no Frame chamado os bytes mais significativos
                //pushOperandStack(tmp_frame->operandStack,value_low);///Empilha  no Frame chamado os bytes menos significativos
            }else{
                value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
                tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
            }
            break;
        case 'J': ///Eh um long
            if(method_descriptor[i-1] != '['){
                value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
                value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
                //tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                    //printf("[J] value_high: %d\t value_low: %d\n",value_high,value_low);
                //pushOperandStack(tmp_frame->operandStack,value_high);///Empilha  no Frame chamado os bytes mais significativos
                //pushOperandStack(tmp_frame->operandStack,value_low);///Empilha  no Frame chamado os bytes menos significativos
            }else{
                value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
                tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
            }
            break;
        case '[':
                break;
        default:
            value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
            tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
        }
        num_parans++;
     }

//     ///Limpa os dados do vetor de variaveis locais
//     for(int i = 1; i < aux_localVariableArray_index; i++){
//        tmp_frame->localVariableArray[i].value = 0;
//     }
     pushFrameStack(frameStackTop, cur_frame); ///Empilha o Frame Corrente
     *cur_frame = *tmp_frame; ///Muda o Frame corrente para o novo Frame
     *curPC = 0;
//    system("pause");
}

void  doInvokespecial(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 *curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction){
     ///Carrega as informacoes da Classe e Metodo
     u2 method_ref = code[*curPC+1];
     method_ref = method_ref << BYTE_SIZE | code[*curPC+2];
     u2 class_index = cur_frame->constant_pool[method_ref].Methodref.class_index;
     u2 class_name_index = cur_frame->constant_pool[class_index].Class.name_index;
     u2 name_and_type_index = cur_frame->constant_pool[method_ref].Methodref.name_and_type_index;
     u2 name_index = cur_frame->constant_pool[name_and_type_index].NameAndType.name_index;
     u2 descriptor_index = cur_frame->constant_pool[name_and_type_index].NameAndType.descriptor_index;
     u1 * method_descriptor = cur_frame->constant_pool[descriptor_index].UTF8.bytes; ///Descricoa do metodo
     u1 * method_name = cur_frame->constant_pool[name_index].UTF8.bytes; ///Nome do metodo
     u1 * class_name = cur_frame->constant_pool[class_name_index].UTF8.bytes; ///Nome da classe

//     printf("method_descriptor: %s\n",method_descriptor);
//     printf("method_name: %s\n",method_name);
//     printf("class_name: %s\n",class_name);

    ///Caso seja necessario, carrega nova classe no HEAP
    u4 method_index;

    ///Caso o nome da classe dona do novo metodo seja diferente da classe corrente.
    u4 new_class_index_heap = loadNewClass(class_file,numberOfClassesHeap,class_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);


    ///Procura pelo indice do metodo no method_info da classe
    method_index = seekNewMethodInClassHandler(handler+new_class_index_heap, method_name,method_descriptor); ///Pega o indice do metodo no method_info da classe.
//    printf("Teste 2\n");
    createNewFrame(handler+new_class_index_heap,method_index, *curPC+3,frameStackTop); ///Cria novo Frame para o novo metodo

    ///Cria o Novo Frame
    Frame * tmp_frame;
    tmp_frame = (Frame *)malloc(sizeof(Frame));
    tmp_frame = popFrameStack(frameStackTop);///Retira o novo frame da pilha

    ///Coloca os parametros da pilha de operandos do Frame corrente.
     u2 method_descriptor_len = strlen(method_descriptor);
     int num_parans = 0;
     int localVariableArray_index = 0;
     u4 value_low,value_high,value;

     for(int i = method_descriptor_len-3; i >= 0; i--){
        if(method_descriptor[i] != '(' && method_descriptor[i] != ')' && method_descriptor[i] != 'V' && method_descriptor[i] != '['){
            if(method_descriptor[i] == 'J' || method_descriptor[i] == 'D'){
                num_parans +=2;
            }else{
               num_parans++;
            }
        }
     }

     localVariableArray_index = num_parans-1;
     num_parans = 0;

     for(int i = method_descriptor_len-3; i > 0; i--){
        if(method_descriptor[i] == '(')
            break;
        switch(method_descriptor[i]){
        case 'D': ///Eh um double
            value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
            value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
            //tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
            tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
            tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
            break;
        case 'J': ///Eh um long
            value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
            value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
            //tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
            tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
            tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
            break;
        default:
            value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
            tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
            break;
        }
        num_parans++;
     }

      pushFrameStack(frameStackTop, cur_frame); ///Empilha o Frame Corrente
     *cur_frame = *tmp_frame; ///Muda o Frame corrente para o novo Frame
     *curPC = 0;

}

void  doInvokeinterface(Frame *cur_frame,StructFrameStack *frameStackTop,ClassHandler * handler, u4 curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction){
     ///Carrega as informacoes da Classe e Metodo
     u2 method_ref = code[curPC+1];
     method_ref = method_ref << BYTE_SIZE | code[curPC+2];
     u2 class_index = cur_frame->constant_pool[method_ref].Methodref.class_index;
     u2 class_name_index = cur_frame->constant_pool[class_index].Class.name_index;
     u2 name_and_type_index = cur_frame->constant_pool[method_ref].Methodref.name_and_type_index;
     u2 name_index = cur_frame->constant_pool[name_and_type_index].NameAndType.name_index;
     u2 descriptor_index = cur_frame->constant_pool[name_and_type_index].NameAndType.descriptor_index;
     u1 * method_descriptor = cur_frame->constant_pool[descriptor_index].UTF8.bytes; ///Descricoa do metodo
     u1 * method_name = cur_frame->constant_pool[name_index].UTF8.bytes; ///Nome do metodo
     u1 * class_name = cur_frame->constant_pool[class_name_index].UTF8.bytes; ///Nome da classe

//     printf("method_descriptor: %s\n",method_descriptor);
//     printf("method_name: %s\n",method_name);
//     printf("class_name: %s\n",class_name);

    ///Caso seja necessario, carrega nova classe no HEAP
    Frame * tmp_frame;// = (Frame *)malloc(sizeof(Frame));
    u4 method_index;
    u2 this_class_index = cur_frame->handler->classRef->this_class;
    u2 this_class_name_index = cur_frame->constant_pool[this_class_index].Class.name_index;
    u1 this_class_name;

    if(this_class_name_index != class_name_index){
            printf("Classe nova!\n");
         this_class_name = cur_frame->constant_pool[this_class_name_index].UTF8.bytes; ///Nome da classe dona do Frame corrente.
        ///Caso o nome da classe dona do novo metodo seja diferente da classe corrente.
        u4 new_class_index_heap = loadNewClass(class_file,numberOfClassesHeap,class_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);
        printf("method_name: %s\t method_descriptor: %s\n",method_name,method_descriptor);
        ///Procura pelo indice do metodo no method_info da classe
        method_index = seekNewMethodInClassHandler(handler+new_class_index_heap, method_name,method_descriptor); ///Pega o indice do metodo no method_info da classe.
        createNewFrame(handler+new_class_index_heap,method_index, curPC+3,frameStackTop); ///Cria novo Frame para o novo metodo
    }else{
        ///Caso o metodo chamado seja da mesma classe do metodo chamador
        ///Procura pelo indice do metodo no method_info da classe
        method_index = seekNewMethodInFrameClass(cur_frame, method_name,method_descriptor); ///Pega o indice do metodo no method_info da classe.
        u4 class_index_heap = loadNewClass(class_file,numberOfClassesHeap,handler->classRef->class_full_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);
        createNewFrame(handler+class_index_heap,method_index, curPC+3,frameStackTop); ///Cria novo Frame para o novo metodo
    }

    ///Cria o Novo Frame
    tmp_frame = (Frame *)malloc(sizeof(Frame));
        tmp_frame = popFrameStack(frameStackTop);///Retira o novo frame da pilha


    ///Coloca os parametros da pilha de operandos do Frame corrente.
     u2 method_descriptor_len = strlen(method_descriptor);
     int num_parans = 0;
     int localVariableArray_index = 0;
     u4 value_low,value_high,value;

    for(int i = method_descriptor_len-3; i >= 0; i--){
        if(method_descriptor[i] != '(' && method_descriptor[i] != ')' && method_descriptor[i] != 'V' && method_descriptor[i] != '['){
            if(method_descriptor[i] == 'J' || method_descriptor[i] == 'D'){
                num_parans +=2;
            }else{
               num_parans++;
            }
        }
     }

     localVariableArray_index = num_parans-1;
     num_parans = 0;

     for(int i = method_descriptor_len-3; i >= 0; i--){
        switch(method_descriptor[i]){
        case 'D': ///Eh um double
            if(method_descriptor[i-1] != '['){
                value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
                value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
                //pushOperandStack(tmp_frame->operandStack,value_high);///Empilha  no Frame chamado os bytes mais significativos
                //pushOperandStack(tmp_frame->operandStack,value_low);///Empilha  no Frame chamado os bytes menos significativos
            }else{
                value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
                tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
            }
            break;
        case 'J': ///Eh um long
            if(method_descriptor[i-1] != '['){
                value_low = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes menos significativos
                value_high = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador os bytes mais significativos
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_high; ///Salva temporariamente no vetor de variaveis locais do novo frame
                tmp_frame->localVariableArray[localVariableArray_index--].value = value_low;
                //pushOperandStack(tmp_frame->operandStack,value_high);///Empilha  no Frame chamado os bytes mais significativos
                //pushOperandStack(tmp_frame->operandStack,value_low);///Empilha  no Frame chamado os bytes menos significativos
            }else{
                value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
                tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
            }
            break;
        case '[':
                break;
        case ')':
        case '(':
            break;
        default:
            value = popOperandStack(cur_frame->operandStack);///Desenpilha do Frame chamador
            tmp_frame->localVariableArray[localVariableArray_index--].value = value; ///Salva temporariamente no vetor de variaveis locais do novo frame
        }
        num_parans++;
     }

//     ///Limpa os dados do vetor de variaveis locais
//     for(int i = 1; i < aux_localVariableArray_index; i++){
//        tmp_frame->localVariableArray[i].value = 0;
//     }
     pushFrameStack(frameStackTop, cur_frame); ///Empilha o Frame Corrente
     *cur_frame = *tmp_frame; ///Muda o Frame corrente para o novo Frame
//    system("pause");
}

void doNew(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 curPC, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction){
     ///Carrega as informacoes da Classe e Metodo
     u2 class_index = code[curPC+1];
     class_index = class_index << BYTE_SIZE | code[curPC+2];
     u2 class_name_index = cur_frame->constant_pool[class_index].Class.name_index;
     u1 * class_name = cur_frame->constant_pool[class_name_index].UTF8.bytes; ///Nome da classe

    ///Caso o nome da classe dona do novo metodo seja diferente da classe corrente.
    u4 new_class_index_heap = loadNewClass(class_file,numberOfClassesHeap,class_name,handler,numberOfClasses, frameStackTop,numberOfByteInstruction);
    pushOperandStack(cur_frame->operandStack,class_file+new_class_index_heap);
}

void doInstructionInvoke(Frame *cur_frame, StructFrameStack *frameStackTop, ClassHandler * handler, u4 * curPC, u1 flagIsWide, u1 * code, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, u1 * numberOfByteInstruction){
    u1 aux_u1, aux2_u1;
	u2 aux_u2, index;
	int16_t branchoffset;
	u4 aux_u4, aux2_u4, aux3_u4, aux4_u4, returnAddress, opcodeAddress;
	u8 aux_u8, aux2_u8;
	float aux_f, aux2_f;
	double aux_d, aux2_d;

    switch(code[*curPC]){
		case OPCODE_invokevirtual:
		    instr_invokeVirtual(cur_frame, *curPC, flagIsWide, code);
			break;
		case OPCODE_invokespecial:
		    doInvokespecial(cur_frame,frameStackTop,cur_frame->handler, curPC, flagIsWide, code, class_file, numberOfClassesHeap, numberOfClasses, numberOfByteInstruction);
			break;
		case OPCODE_invokestatic:
		    doInvokestatic(cur_frame,frameStackTop,cur_frame->handler, curPC, flagIsWide, code, class_file, numberOfClassesHeap, numberOfClasses, numberOfByteInstruction);
			break;
		case OPCODE_invokeinterface:
            doInvokeinterface(cur_frame,frameStackTop,cur_frame->handler, *curPC, flagIsWide, code, class_file, numberOfClassesHeap, numberOfClasses, numberOfByteInstruction);
			break;
        case OPCODE_new:
            doNew(cur_frame, frameStackTop, handler, *curPC, code, class_file, numberOfClassesHeap, numberOfClasses, numberOfByteInstruction);
            break;
    }
}

void doCLInit(ClassFile * class_file,u1 * code, Frame * clinit_frame, u1 * numberOfByteInstruction){
    u4 curPC = 0;
    u1 curOPCODE, flagIsWide = 0;
    u4 attributeCodeIndex = findAttributeCodeIndex(clinit_frame->methods->attributes,clinit_frame->methods->attributes_count);
    while(code[curPC]){
        curOPCODE = getOpcode(&clinit_frame->methods->attributes[attributeCodeIndex], curPC); ///Procura pelo OPCODE apontado por curPC.
        if(curOPCODE == OPCODE_wide){
            flagIsWide = 1; ///Seta a flag avisando que a proxima instrucao eh do tipo WIDE.
        }else{
             doInstruction(clinit_frame, curPC, flagIsWide, clinit_frame->methods->attributes[attributeCodeIndex].Code.code);
        }
        ///Incrementa PC
        incPC(&curPC,curOPCODE,numberOfByteInstruction);
        flagIsWide = 0;
    }
}

void doInstructionArray(Frame * frame, u4 pc, u1 fWide, u1 * code){
    u1 atype;
    u2 aux_u2;
    u4 index, value_u4, value2_u4;
    u8 value_u8;
    int count, element_size;
    struct_Array *array;

    switch(code[pc]){

        /// Operacoes de 32 bits
        case OPCODE_iaload: ///Load int from array
		case OPCODE_faload: ///Load float from array
		case OPCODE_aaload: ///Load reference from array
		    index = popOperandStack(frame->operandStack);
		    array = popOperandStack(frame->operandStack);
		    value_u4 = ((u4*)array->data)[index];///conversao de void para 32 bits
		    pushOperandStack(frame->operandStack, value_u4);
			break;

        /// Operacoes de 64 bits
		case OPCODE_laload: ///Load long from array
		    index = popOperandStack(frame->operandStack);
		    array = popOperandStack(frame->operandStack);
		    value_u8 = ((u8*)array->data)[index]; ///conversao de void par     64 bits
		    value_u4 = (u4)(value_u8 & 0xFFFFFFFF); ///Pega a segunda metade do numero de 64 bits para colocar na pilha
		    pushOperandStack(frame->operandStack, value_u4);
		    value_u4 = (u4)(value_u8 >> 32); ///Pega a primeira metade do numero de 64 bits para colocar na pilha
		    pushOperandStack(frame->operandStack, value_u4);
		    break;
		case OPCODE_daload: ///Load double from array
		    index = popOperandStack(frame->operandStack);
		    array = popOperandStack(frame->operandStack);
		    value_u8 = ((u8*)array->data)[index]; ///conversao de void par     64 bits
		    value_u4 = (u4)(value_u8 >> 32); ///Pega a segunda metade do numero de 64 bits para colocar na pilha
		    pushOperandStack(frame->operandStack, value_u4);
		    value_u4 = (u4)(value_u8 & 0xFFFFFFFF); ///Pega a primeira metade do numero de 64 bits para colocar na pilha
		    pushOperandStack(frame->operandStack, value_u4);
			break;

        /// Operacoes de 8 bits
		case OPCODE_baload: ///Load byte or boolean from array
		    index = popOperandStack(frame->operandStack);
		    array = (struct_Array*)popOperandStack(frame->operandStack); ///Converter para o tipo da struct Array
		    value_u4 = (u4)((u1*)(array->data))[index];///recupera o valor e expande o valor para 32 bits
		    pushOperandStack(frame->operandStack, value_u4);
			break;

        /// Operacoes de 16 bits
		case OPCODE_caload: ///Load char from array
		case OPCODE_saload: ///Load short from array
		    index = popOperandStack(frame->operandStack);
		    array = (struct_Array*)popOperandStack(frame->operandStack);
		    value_u4 = (u4)((u2*)(array->data))[index];///recupera o valor e expande o valor para 32 bits
		    pushOperandStack(frame->operandStack, value_u4);
			break;

        ///Operacoes de 32 bits
        case OPCODE_iastore: ///Store into int array
        case OPCODE_aastore: ///Store into reference array
		case OPCODE_fastore: ///Store into float array
		    value_u4 = popOperandStack(frame->operandStack);
            index = popOperandStack(frame->operandStack);
            array = (struct_Array*)popOperandStack(frame->operandStack);
            ((u4*)(array->data))[index] = value_u4;
			break;

        ///Operacoes de 64 bits
        case OPCODE_lastore: ///Store into long array
		case OPCODE_dastore: ///Store into double array
		    value_u4 = popOperandStack(frame->operandStack);
		    value2_u4 = popOperandStack(frame->operandStack);
		    index = popOperandStack(frame->operandStack);
		    array = popOperandStack(frame->operandStack);
		    value_u8 = value_u4;
			value_u8 = (value_u8 << 32) | value2_u4; ///Concatena os dois valores retornados pela pilha
			((u8*)array->data)[index] = value_u8; ///Armazena o valor recuperado na pilha
			break;

        ///Operacoes de 16 bits
        case OPCODE_castore: ///Store into char array
        case OPCODE_sastore: ///Store into short array
            value_u4 = popOperandStack(frame->operandStack);
            index = popOperandStack(frame->operandStack);
            array = popOperandStack(frame->operandStack);
			((u2*)(array->data))[index]  = (u2)value_u4; ///Converte o valor encontrado em 8 bits com sinal e insere no array
			break;

        case OPCODE_bastore: ///Store into byte or boolean array
            value_u4 = popOperandStack(frame->operandStack);
            index = popOperandStack(frame->operandStack);
            array = popOperandStack(frame->operandStack);
			((u1*)array->data)[index]  = (u1)value_u4; ///Converte o valor encontrado em 8 bits com sinal e insere no array
			break;

        case OPCODE_newarray:
            atype = code[pc+1]; ///recebe o tipo do array
            count = popOperandStack(frame->operandStack); ///Tamanho do array

            switch(atype){
                case (4): ///Boolean
                    element_size = sizeof(int8_t); ///Representado como 1 byte
                    //array[count] = '*';
                    break;
                case (5): ///Char
                    element_size = sizeof(char);
                    break;
                case (6): ///Float
                    element_size = sizeof(float);
                    break;
                case (7): ///Dobble
                    element_size = sizeof(double);
                    break;
                case (8): ///Byte
                    element_size = sizeof(int8_t); ///lembrando que tem sinal
                    break;
                case (9): ///Short
                    element_size = sizeof(int16_t);
                    break;
                case (10): ///Inteiro
                    element_size = sizeof(int32_t);
                    break;
                case (11): ///Long
                    element_size = sizeof(int64_t);
                    break;
            }
            array = (struct_Array*)malloc(sizeof(struct_Array));
            array->data = calloc(count, element_size);
            array->length = count;
//printf("%p\n",frame->handler->arrayList->array_t);
            frame->handler->arrayList->array_t = (array); ///Salva o array no handler

            pushArrayList(frame->handler->arrayList, *frame->handler->arrayList->array_t);

            pushOperandStack(frame->operandStack, frame->handler->arrayList->array_t);
            //printStack(frame->operandStack);
            //getchar();
			break;
///PODE HAVER ERRO AQUI-------------------------------------------------------------------------------------------
		case OPCODE_anewarray:  ///Create new array of reference
		    aux_u2 = (code[pc+1] << 8) | code[pc+2];
		    index = (u4)aux_u2; ///indice no constant pool corrente  que contem uma referencia simbólica para a o tipo da classe/array/interface

            count = popOperandStack(frame->operandStack); ///Tamanho do array

            array->data = (ClassHandler*)calloc(count, sizeof(ClassHandler));
            array->length = count;

            frame->handler->arrayList->array_t = array;

            pushArrayList(frame->handler->arrayList, *frame->handler->arrayList->array_t);

            pushFrameStack(frame->operandStack, array);
			break;
		case OPCODE_arraylength: ///Get length of array
		    array = (struct_Array*)popOperandStack(frame->operandStack);   ///recupera a struct que contem o array e seu tamanho

		    pushOperandStack(frame->operandStack, array->length); ///armazena o tamanho do array na pilha
			break;
        case OPCODE_multianewarray: ///Create new multidimensional array

			break;
    }
}
