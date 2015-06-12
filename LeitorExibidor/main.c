#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/UserViewer.h"

void fillNumberOfByteInstruction(u1 * numberOfByteInstruction);

int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    char *program_full_name = argv[0]; /* nome do programa para caso de erro */
    u4 PC;
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        throwException(OPEN_FILE_ERROR,OPEN_FILE_ERROR_MSG);
    }
    printf("Lendo o bytecode Java para a memoria...\n");
    class_file = classRead(dot_class);
    printf("Bytecode Java copiado com sucesso!\n");
    fclose(dot_class);

    if(argc > 2 && !strcmp(*++argv,"-print")) ///Verifica se a flag para printar o .class foi passado pela linha de comando.
        classPrint(class_file);


    fillNumberOfByteInstruction(&numberOfByteInstruction);

    return 0;
}

void fillNumberOfByteInstruction(u1 * numberOfByteInstruction){
    for(int i = 0; i < MAX_INSTRUCTIONS; i++){
        switch(i){
            case OPCODE_bipush:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_ldc:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_iload:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_lload:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_fload:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_dload:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_aload:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_istore:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_lstore:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_fstore:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_dstore:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_astore:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_ret:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_newarray:
                numberOfByteInstruction[i] = 2; ///2 bytes
                break;
            case OPCODE_sipush:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ldc_w:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ldc2_w:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_iinc:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifeq:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifne:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_iflt:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifge:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifgt:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifle:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmpeq:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmpne:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmplt:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmpge:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmpgt:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_icmple:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_if_acmpeq:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_goto:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_jsr:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_getstatic:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_putstatic:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_getfield:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_putfield:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_invokevirtual:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_invokespecial:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_invokestatic:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_invokeinterface:
                numberOfByteInstruction[i] = 5; ///ATENÇÃO! Isso aqui pode estar errado...
                break;
            case OPCODE_new:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_anewarray:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_checkcast:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_instanceof:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_multianewarray:
                numberOfByteInstruction[i] = 4; ///4 bytes
                break;
            case OPCODE_ifnull:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_ifnonnull:
                numberOfByteInstruction[i] = 3; ///3 bytes
                break;
            case OPCODE_goto_w:
                numberOfByteInstruction[i] = 5; ///ATENÇÃO! Isso aqui pode estar errado...
                break;
            case OPCODE_jsr_w:
                numberOfByteInstruction[i] = 5; ///ATENÇÃO! Isso aqui pode estar errado...
                break;
            default:
                numberOfByteInstruction[i] = 1; ///1 byte
                break;
        }
    }

}



