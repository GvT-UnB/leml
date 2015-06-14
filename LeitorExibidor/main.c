#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/UserViewer.h"
#include "lib/frameManager.h"

typedef struct StructFrameStack{
    Frame frame;
    struct StructFrameStack *next;
}StructFrameStack;

/** \brief Completa o vetor numberOfByteInstruction com a quantidade de bytes necessarias para cada instrução
 *
 * \param numberOfByteInstruction u1* Vetor que armazena a quantidade de bytes que cada instrução utiliza.
 * \return void
 *
 */
void fillNumberOfByteInstruction(u1 * numberOfByteInstruction);

/** \brief Verifica se o bytecode informado tem o mesmo nome que o indicado dentro do bytecode.
 *
 * \param argv char* nome do bytecode
 * \param class_file ClassFile* espaço em memoria onde os daados da classe estão salvos.
 * \return void
 *
 */
void verifyClassName(char * argv, ClassFile * class_file);
///Funcoes para o frame, ainda precisam de ajustes
///-----------------------------------------------------------------------------------------------------------------------
void createClassFrames(ClassFile *class_file, StructFrameStack **frameStackTop);
void loadClassFrames(StructFrameStack **frameStackTop);
void pushFrameStack(StructFrameStack **frameStackTop, Frame frame);
Frame popFrameStack(StructFrameStack **frameStackTop);
///-----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    ClassHandler * handler;
    StructFrameStack *frameStackTop;
    //char *program_full_name = argv[0]; /* nome do programa para caso de erro */
    u4 PC;
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.

    frameStackTop = NULL;

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        throwException(OPEN_FILE_ERROR,OPEN_FILE_ERROR_MSG);
    }
    printf("Lendo o bytecode Java para a memoria...\n");
    class_file = classRead(dot_class);
    printf("Bytecode Java copiado com sucesso!\n");
    fclose(dot_class);

    verifyClassName(*argv,class_file);

    if(argc > 2 && !strcmp(*++argv,"-print")) ///Verifica se a flag para printar o .class foi passado pela linha de comando.
        classPrint(class_file);

    fillNumberOfByteInstruction(&numberOfByteInstruction);

    newObject(handler,class_file); ///Instancia um novo Objeto da classe class_file

    ///Funcoes para a manipulacao e montagem da pilha de frames e CRIACAO DE CADA FRAME (ainda nao feito)
    createClassFrames(class_file, &frameStackTop);
    loadClassFrames(&frameStackTop);

    return 0;
}
///Area para manipulacao da pilha de frames - AINDA TEM QUE AJUSTAR
///-----------------------------------------------------------------------------------------------------------------------
void createClassFrames(ClassFile *class_file, StructFrameStack **frameStackTop){
    Frame frame;
    ///Loop para criar a quantidade de frames do .class e inserir na pilha de frames
    for(int i=0; i < class_file->methods_count; i++){
        ///Funcao para criar o frame - FAZER
        ///A cada frame criado, usa-se a função pushFrameStack para colocá-lo na pilha
        pushFrameStack(frameStackTop, frame);
    }
}

void loadClassFrames(StructFrameStack **frameStackTop){
    Frame currentFrame; ///Frame corrente (frame em execucao)
    while((*frameStackTop)->next != NULL){
        ///Para utilizar um frame da pilha, primeiro se tira ele da pilha
        currentFrame = popFrameStack(frameStackTop);
        ///Chama a funcao para operar o frame - FAZER
    }
}

void pushFrameStack(StructFrameStack **frameStackTop, Frame frame){
    StructFrameStack *nodeFrame;
    nodeFrame = (StructFrameStack*)malloc(sizeof(StructFrameStack));
    nodeFrame->next = *frameStackTop;
    nodeFrame->frame = frame;
    *frameStackTop = nodeFrame;
}

Frame popFrameStack(StructFrameStack **frameStackTop){
    StructFrameStack *aux;
    Frame currentframe;
    aux = *frameStackTop;
    *frameStackTop = (*frameStackTop)->next;
    currentframe = aux->frame;
    free(aux);
    return currentframe;
}
///-----------------------------------------------------------------------------------------------------------------------
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


void verifyClassName(char * argv, ClassFile * class_file){
    int i=0;
    int classNameLength = strlen(argv);
    char hasSlash;
    char classRealName[100];
    char * className;
    char * classFullName = (char *)malloc(classNameLength * sizeof(char *));
    classFullName = argv;

    ///Retira o caminho até o arquivo
    while(hasSlash = strchr(classFullName,'/')){
        while(classFullName[i] != '/'){
            i++;
        }
        className = (char *)malloc((classNameLength-i) * sizeof(char *));
        i++;
        for(int j=0;j<=(classNameLength-i);j++){
            className[j] = classFullName[i+j];
        }
        strcpy(classFullName,className);
        classNameLength = strlen(classFullName);
    }

    ///Retira o .class do nome do arquivo
    i=0;
    while(classFullName[i] != '.'){
        i++;
    }
    free(className);
    className = (char *)malloc((i+1) * sizeof(char *));
    for(int j=0;j<i;j++){
        className[j] = classFullName[j];
    }
    className[i] = '\0';

    ///Verifica se o nomo do arquivo é o mesmo nome da classe
    selectPointer(class_file, class_file->this_class, classRealName, 0);
    if(strcmp(className,classRealName)){
        throwException(CLASS_DIFFER_FILE_NAME,CLASS_DIFFER_FILE_NAME_MSG);
    }
}

