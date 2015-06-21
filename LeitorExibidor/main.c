#define JVMMANAGER_SERV
#include "lib/jvmManager.h"

///TODO: Tratar os fields estaticos,são inicializados com ZERO
///TODO: Criar o ClassLoader, ele precisa verificar o PATH da classe
///TODO: Array com Union
///TODO: handler estah errado, necessita tratamento similar ao do class_file
///TODO: Empilhar o frame corrente antes de criar um novo frame.
///TODO: Criar função que carrega novo metodo, ele recebe entrada do tipo NOME_DA_CLASSE.NOME_METODO e procura pro NOME_METODO no method_info da classe.


int main(int argc, char *argv[]){
    ClassFile * class_file = (ClassFile *)malloc(MAX_CLASSES_ON_HEAP*sizeof(ClassFile)); ///Eh o HEAP...pois eh, nao tah mnemônico :/
    ClassHandler * handler = (ClassHandler *)malloc(sizeof(ClassHandler));
    StructFrameStack *frameStackTop = (StructFrameStack *)malloc(sizeof(StructFrameStack));
    frameStackTop->next = NULL;
    u4 curPC = 0;///==============================MODIFIQUEI AQUI===========================================================================================
    u4 numberOfClasses = 0; ///Quantidade de objetos instanciados
    u4 numberOfClassesHeap = 0; ///Quantidade de classes na memoria
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.

    ///Carrega a classe informada por linha de comando no HEAP
    classLoader(class_file+numberOfClassesHeap, *++argv, &numberOfClassesHeap);
    //classPrint(class_file+numberOfClassesHeap-1);

    if(argc > 2 && !strcmp(*++argv,"-print")) ///Verifica se a flag para printar o .class foi passado pela linha de comando.
        classPrint(class_file+numberOfClassesHeap-1);

    fillNumberOfByteInstruction(&numberOfByteInstruction);

    ///Instancia um novo Objeto.
    createNewObject(handler,&numberOfClasses,class_file+numberOfClassesHeap-1 );
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
    runJVM(cur_frame,&curPC,&numberOfByteInstruction, frameStackTop);

    ///Verifica se uma classe ja esta no HEAP
        classLoader(class_file+numberOfClassesHeap, "tutorial/HelloWorld.class", &numberOfClassesHeap);
/*            printf("[1] numberOfClassesHeap - 1 = %d\n",(numberOfClassesHeap-1));
            classPrint(class_file+numberOfClassesHeap-1);
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            system("pause");*/
        createNewObject(handler,&numberOfClasses,class_file+numberOfClassesHeap-1);

        classLoader(class_file+numberOfClassesHeap, "tutorial/OrdenarArray.class", &numberOfClassesHeap);
/*        classPrint(class_file+numberOfClassesHeap-1);
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            system("pause");*/
        createNewObject(handler,&numberOfClasses,class_file+numberOfClassesHeap-1);

        classLoader(class_file+numberOfClassesHeap, "tutorial/Teste.class", &numberOfClassesHeap);
 /*           printf("[2] numberOfClassesHeap - 1 = %d\n",(numberOfClassesHeap-1));
        classPrint(class_file+numberOfClassesHeap-1);
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            printf("---------------------------------------------------------------------------------------\n");
            system("pause");*/
        //createNewObject(handler,&numberOfClasses,class_file+numberOfClassesHeap-1);


    //u1 * className = "tutorial/OrdenarArray"; ///Pega o nome da Classe.
    //u1 * className = "tutorial/TiposPrimitivos"; ///Pega o nome da Classe.
    //u1 * className = "tutorial/Teste"; ///Pega o nome da Classe.
    u1 * className = "tutorial/ClassicSingleton"; ///Pega o nome da Classe.
    u4 class_index = loadNewClass(class_file,&numberOfClassesHeap,className,handler,&numberOfClasses);
printf("numberOfClassesHeap: %d\tnumberOfClasses: %d\n",numberOfClassesHeap,numberOfClasses);
printf("class_index: %d\n",class_index);
    return 0;
}


void runJVM(Frame * cur_frame,u4 * curPC, u1 * numberOfByteInstruction, StructFrameStack *frameStackTop){
    u1 curOPCODE, flagIsWide = 0;
    //cur_frame->operandStack->next = NULL;
    u4 attributeCodeIndex = findAttributeCodeIndex(cur_frame->methods->attributes,cur_frame->methods->attributes_count); ///Localiza o indice do attriute Code
    while(cur_frame->methods->attributes[attributeCodeIndex].Code.code[*curPC]){ ///Enquanto o code nao for NULL, repete
        //printf("PC: %d\n",*curPC);
        curOPCODE = getOpcode(&cur_frame->methods->attributes[attributeCodeIndex], *curPC); ///Procura pelo OPCODE apontado por curPC.
        if(curOPCODE == OPCODE_wide){
            flagIsWide = 1; ///Seta a flag avisando que a proxima instrucao eh do tipo WIDE.
            ///Incrementa PC
            incPC(curPC,curOPCODE,numberOfByteInstruction);
        }else{
            ///Chama a funcao que realiza as instrucoes. Esta sendo implementada pelo GVT.
            printf("OPCODE: %d\tPC: %d\n", curOPCODE, *curPC);
            if(curOPCODE > 152 && curOPCODE < 178){
                doInstructionShift(cur_frame, curPC, frameStackTop, cur_frame->methods->attributes[attributeCodeIndex].Code.code, flagIsWide);
            }
            else{
                doInstruction(cur_frame, *curPC, flagIsWide, cur_frame->methods->attributes[attributeCodeIndex].Code.code);
                ///Incrementa PC
                incPC(curPC,curOPCODE,numberOfByteInstruction);
            }
            flagIsWide = 0; ///Zera a flag de instrucao WIDE.
        }
        if(*curPC == -1){ ///Programa encerrado. return da main chamado
            break;
        }
        ///Incrementa PC
        ///incPC(curPC,curOPCODE,numberOfByteInstruction);
    }
    //printf("saiu\n");
    printf("PC: %d\n",*curPC);
}

void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file){
    u4 aux = *numberOfClasses +1;
    //printf("TESTE 1: %d\n",aux);
    handler = (ClassHandler *)realloc(handler,aux * sizeof(ClassHandler));///Inicializa o Objeto
    //printf("TESTE 2!\n");
    if(handler == NULL)
        throwException(INSUFFICIENT_MEMORY,INSUFFICIENT_MEMORY_MSG);///Verifica se foi possivel alocar memoria suficiente
    newObject(handler+(*numberOfClasses),class_file); ///Instancia um novo Objeto da classe class_file
    //printf("TESTE 3!\n");
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
    *frameStackTop = *nodeFrame;///Novo membro vira o topo da pilha.
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
        if(frameStackTop->next){
            *frameStackTop = *frameStackTop->next;///Novo topo da fila vira o membro apontado pelo frame retirado.
        }else{
            //printf("Entrei no ELSE!\n");
            free(frameStackTop);
        }
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
            createNewFrame(handler,i,NOT_RETURN,frameStackTop); ///encontrou o metodo main no indice i, cria o frame dele.
            frameStackTop->next = NULL;
            return MAIN_FOUND; ///Retorna sucesso
        }
    }
    throwException(MAIN_NOT_FOUND,MAIN_NOT_FOUND_MSG); ///Nao encontrou a main, taca erro.
}
