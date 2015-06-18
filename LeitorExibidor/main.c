#define JVMMANAGER_SERV
#include "lib/jvmManager.h"

///TODO: Tratar os fields estaticos,são inicializados com ZERO
///TODO: Criar o ClassLoader, ele precisa verificar o PATH da classe
///TODO: Array com Union


int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    ClassHandler * handler = (ClassHandler *)malloc(sizeof(ClassHandler));
    StructFrameStack *frameStackTop = (StructFrameStack *)malloc(sizeof(StructFrameStack));
    u4 curPC = 0;
    u4 numberOfClasses = 0;
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.

    if((dot_class = fopen(*++argv,"rb")) == NULL){
        throwException(OPEN_FILE_ERROR,OPEN_FILE_ERROR_MSG);
    }
    //printf("Lendo o bytecode Java para a memoria...\n");
    class_file = classRead(dot_class);
    //printf("Bytecode Java copiado com sucesso!\n");
    fclose(dot_class);

    verifyClassName(*argv,class_file);

    if(argc > 2 && !strcmp(*++argv,"-print")) ///Verifica se a flag para printar o .class foi passado pela linha de comando.
        classPrint(class_file);

    fillNumberOfByteInstruction(&numberOfByteInstruction);

    ///Instancia um novo Objeto.
    createNewObject(handler,&numberOfClasses,class_file);
    //printHandler(handler);

    ///Procura pela MAIN, caso encontra instancia e coloca no topo da pilha de operandos, caso contrairo taca erro.
    createMainFrame(handler,curPC,frameStackTop);
    //createNewFrame(handler,1,curPC,frameStackTop);
    Frame * cur_frame = (Frame *)malloc(sizeof(Frame));
    cur_frame = popFrameStack(frameStackTop);///Retira o novo frame da pilha
/*
    printf("TESTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("Method Info: \n");
        printf("---------------------------------------------------------------------------------------\n");
        printMethodInfo(cur_frame->methods, 1, cur_frame->handler->classRef);///Apenas para debugar!
        printf("PC de retorno: %d\n",cur_frame->returnPC);
        printf("Constant Pool: \n");
        printf("---------------------------------------------------------------------------------------\n");
        printConstantPool(cur_frame->constant_pool,cur_frame->handler->classRef->constant_pool_count, cur_frame->handler->classRef);///Apenas para debugar!
*/
    //printHandler(cur_frame->localVariableArray[0].value);

    ///Chama a funcao responsavel por efetivamente rodar a JVM
    runJVM(cur_frame,&curPC,&numberOfByteInstruction);

    ///Verifica se uma classe ja esta no HEAP
    u1 * className = "TiposPrimitivos"; ///Pega o nome da Classe.
    for(int i = 0; i < numberOfClasses; i++){
        if(!strcmp(className,getClassName(handler))){
            printf("Encontrei a classe %s!\n",getClassName(handler+i));
            break;
        }
        printf("Não encontrei :(\n");
    }
    printf("%s\n",className);

    return 0;
}


void runJVM(Frame * cur_frame,u4 * curPC, u1 * numberOfByteInstruction){
    u1 curOPCODE, flagIsWide = 0;
    u4 attributeCodeIndex = findAttributeCodeIndex(cur_frame->methods->attributes,cur_frame->methods->attributes_count); ///Localiza o indice do attriute Code
    while(cur_frame->methods->attributes[attributeCodeIndex].Code.code[*curPC]){ ///Enquanto o code nao for NULL, repete
        //printf("PC: %d\n",*curPC);
        curOPCODE = getOpcode(&cur_frame->methods->attributes[attributeCodeIndex], *curPC); ///Procura pelo OPCODE apontado por curPC.
        if(curOPCODE == OPCODE_wide){
            flagIsWide = 1; ///Seta a flag avisando que a proxima instrucao eh do tipo WIDE.
        }else{
            ///Chama a funcao que realiza as instrucoes. Esta sendo implementada pelo GVT.
        //doInstruction(cur_frame,curOPCODE,flagIsWide);
            flagIsWide = 0; ///Zera a flag de instrucao WIDE.
        }
        ///Incrementa PC
        incPC(curPC,curOPCODE,numberOfByteInstruction);
    }
    printf("PC: %d\n",*curPC);
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
        StructFrameStack * aux = (StructFrameStack*)malloc(sizeof(StructFrameStack));
        Frame * currentframe = (Frame*)malloc(sizeof(Frame));///Instancia um novo frame
        currentframe = frameStackTop->frame;///Novo frame recebe o frame salvo no topo da pilha
        aux = frameStackTop;
        frameStackTop = frameStackTop->next;///Novo topo da fila vira o membro apontado pelo frame retirado.
        free(aux);
        return currentframe;
    }else{
        char stackName = "Pilha de Frames";
        throwException(POP_IN_A_EMPTY_STACK,POP_IN_A_EMPTY_STACK_MSG,stackName);
    }
}

int createMainFrame(ClassHandler * handler,u4 curPC,StructFrameStack *frameStackTop){
    for(int i = 0; i < handler->classRef->methods_count; i++){
        if(!strcmp(handler->classRef->constant_pool[ handler->classRef->methods[i].name_index ].UTF8.bytes,"main")){ ///Procura pelo metodo main no method_info da classe
            //printf("Achei a MAIN no indice %d\n",i);
            createNewFrame(handler,i,curPC,frameStackTop); ///encontrou o metodo main no indice i, cria o frame dele.
            return MAIN_FOUND; ///Retorna sucesso
        }
    }
    throwException(MAIN_NOT_FOUND,MAIN_NOT_FOUND_MSG); ///Nao encontrou a main, taca erro.
}
