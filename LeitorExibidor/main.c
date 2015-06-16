#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/UserViewer.h"
#include "lib/frameManager.h"

///Funcoes para o frame, ainda precisam de ajustes
///-----------------------------------------------------------------------------------------------------------------------
//void createClassFrames(ClassFile *class_file, StructFrameStack **frameStackTop);
void loadClassFrames(StructFrameStack **frameStackTop);
///-----------------------------------------------------------------------------------------------------------------------

/** \brief Completa o vetor numberOfByteInstruction com a quantidade de bytes necessarias para cada instru��o
 *
 * \param numberOfByteInstruction u1* Vetor que armazena a quantidade de bytes que cada instru��o utiliza.
 * \return void
 *
 */
void fillNumberOfByteInstruction(u1 * numberOfByteInstruction);

/** \brief Verifica se o bytecode informado tem o mesmo nome que o indicado dentro do bytecode.
 *
 * \param argv char* nome do bytecode
 * \param class_file ClassFile* espa�o em memoria onde os daados da classe est�o salvos.
 * \return void
 *
 */
void verifyClassName(char * argv, ClassFile * class_file);

/** \brief Instancia um novo objeto da classe class_file.
 *
 * \param handler ClassHandler* Objeto que ser� criado.
 * \param numberOfClasses u4* Quantidade de classes instanciadas.
 * \param class_file ClassFile* Classe que ser� instanciada.
 * \return void
 *
 */
void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file);

/** \brief Instancia um novo Frame de um metodo da classe class_file e logo em seguida o insere no topo da pilha de Frames.
 *
 * \param handler ClassHandler* Objeto dono do metodo.
 * \param method_index u4 Indice do metodo no methods do class_file.
 * \param curPC u4 PC Corrente.
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de Frames.
 * \return void
 *
 */
void createNewFrame(ClassHandler * handler, u4 method_index, u4 curPC,StructFrameStack *frameStackTop);

/** \brief Coloca o frame no topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o corrente topo da pilha de frames.
 * \param frame Frame* referencia para o frame que sea inserido no topo da pilha de frames.
 * \return void
 *
 */
void pushFrameStack(StructFrameStack *frameStackTop, Frame * frame);

/** \brief Retira um frame do topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de frames.
 * \return Frame* Referencia para o frame retirado do topo da pilha.
 *
 */
Frame * popFrameStack(StructFrameStack *frameStackTop);

int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    ClassHandler * handler = (ClassHandler *)malloc(sizeof(ClassHandler));
    StructFrameStack *frameStackTop = (StructFrameStack *)malloc(sizeof(StructFrameStack));
    u4 curPC = 0;
    u4 numberOfClasses = 0;
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instru��o utiliza.

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

    ///Instancia um novo Objeto.
    createNewObject(handler,&numberOfClasses,class_file);
    //printHandler(handler);

    ///ATEN��O!!!! Estou enviando o valor fixo de 1 pois no MOMENTO vamos tratar apenas o metodo de indice 1 (geralmente o metodo main)
    createNewFrame(handler,1,curPC,frameStackTop); ///Cria um novo Frame e o coloca na pilha
    Frame * newFrame = (Frame *)malloc(sizeof(Frame));
    newFrame = popFrameStack(frameStackTop);///Retira o novo frame da pilha
    printf("TESTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("Method Info: \n");
        printf("---------------------------------------------------------------------------------------\n");
        printMethodInfo(newFrame->localVariableArray[0], 1, newFrame->handler->classRef);///Apenas para debugar!
        printf("PC de retorno: %d\n",newFrame->returnPC);
        printf("Constant Pool: \n");
        printf("---------------------------------------------------------------------------------------\n");
        printConstantPool(newFrame->constant_pool,newFrame->handler->classRef->constant_pool_count, newFrame->handler->classRef);///Apenas para debugar!

    ///Funcoes para a manipulacao e montagem da pilha de frames e CRIACAO DE CADA FRAME (ainda nao feito)
//    createClassFrames(class_file, &frameStackTop);
    //loadClassFrames(&frameStackTop);

    return 0;
}
///Area para manipulacao da pilha de frames - AINDA TEM QUE AJUSTAR
///-----------------------------------------------------------------------------------------------------------------------

void loadClassFrames(StructFrameStack **frameStackTop){
    Frame currentFrame; ///Frame corrente (frame em execucao)
    while((*frameStackTop)->next != NULL){
        ///Para utilizar um frame da pilha, primeiro se tira ele da pilha
        //currentFrame = popFrameStack(frameStackTop);
        ///Chama a funcao para operar o frame - FAZER
    }
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
                numberOfByteInstruction[i] = 5; ///ATEN��O! Isso aqui pode estar errado...
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
                numberOfByteInstruction[i] = 5; ///ATEN��O! Isso aqui pode estar errado...
                break;
            case OPCODE_jsr_w:
                numberOfByteInstruction[i] = 5; ///ATEN��O! Isso aqui pode estar errado...
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

    ///Retira o caminho at� o arquivo
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

    ///Verifica se o nomo do arquivo � o mesmo nome da classe
    selectPointer(class_file, class_file->this_class, classRealName, 0);
    if(strcmp(className,classRealName)){
        throwException(CLASS_DIFFER_FILE_NAME,CLASS_DIFFER_FILE_NAME_MSG);
    }
}

void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file){
    u4 aux = *numberOfClasses +1;
    handler = (ClassHandler *)realloc(handler,aux * sizeof(ClassHandler));///Inicializa o Objeto
    if(handler == NULL)
        throwException(INSUFFICIENT_MEMORY,INSUFFICIENT_MEMORY_MSG);///Verifica se foi possivel alocar memoria suficiente
    newObject(handler+(*numberOfClasses),class_file); ///Instancia um novo Objeto da classe class_file
    //printHandler(handler+(*numberOfClasses));
    *numberOfClasses = *numberOfClasses + 1; ///Atualiza a quantidade de classes instanciadas.
}

void createNewFrame(ClassHandler * handler, u4 method_index, u4 curPC,StructFrameStack *frameStackTop){
    //Frame * newFrame = (Frame *)malloc(sizeof(Frame));
    Frame * frame = (Frame *)malloc(sizeof(Frame));
    newFrame(frame,handler,method_index,curPC);///Cria o novo metodo de indice method_index referente ao Objeto apontado por handler
    pushFrameStack(frameStackTop, frame);///Coloca o frame no topo da pilha de frames.
}

void pushFrameStack(StructFrameStack *frameStackTop, Frame * frame){
    StructFrameStack *nodeFrame;
    nodeFrame = (StructFrameStack*)malloc(sizeof(StructFrameStack));///Instancia novo membro da pilha de frames
    nodeFrame->next = frameStackTop; ///Novo membro da pilha aponta para o topo da pilha corrente
    nodeFrame->frame = frame;///Salva o frame no novo membro da pilha.
    *frameStackTop = *nodeFrame;///Novo membro vira o topo d apilha.
/*          printf("Method Info: \n");
            printf("---------------------------------------------------------------------------------------\n");
            printMethodInfo(frameStackTop->frame->localVariableArray[0], 1, frameStackTop->frame->handler->classRef);///Apenas para debugar!
            printf("PC de retorno: %d\n",frameStackTop->frame->returnPC);
            printf("Constant Pool: \n");
            printf("---------------------------------------------------------------------------------------\n");
            printConstantPool(frameStackTop->frame->constant_pool,frameStackTop->frame->handler->classRef->constant_pool_count, frameStackTop->frame->handler->classRef);///Apenas para debugar!
*/
}

Frame * popFrameStack(StructFrameStack *frameStackTop){
    if(frameStackTop != NULL){
        Frame * currentframe = (Frame*)malloc(sizeof(Frame));///Instancia um novo frame
        currentframe = frameStackTop->frame;///Novo frame recebe o frame salvo no topo da pilha
        frameStackTop = frameStackTop->next;///Novo topo da fila vira o membro apontado pelo frame retirado.
        //free(aux);
        return currentframe;
    }else{
        printf("TESTE #9\n");
    }
}
