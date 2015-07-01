#define JVMMANAGER_SERV
#include "lib/jvmManager.h"

int main(int argc, char *argv[]){
    ClassFile * class_file = (ClassFile *)malloc(MAX_CLASSES_ON_HEAP*sizeof(ClassFile)); ///Eh o HEAP...pois eh, nao tah mnemônico :/
    ClassHandler * handler = (ClassHandler *)malloc(MAX_CLASSES_ON_HEAP*sizeof(ClassHandler));
    StructFrameStack *frameStackTop = (StructFrameStack *)malloc(sizeof(StructFrameStack));
    //frameStackTop->next = NULL;
    frameStackTop->stackTop = -1;
    u4 curPC = 0;
    u4 numberOfClasses = 0; ///Quantidade de objetos instanciados
    u4 numberOfClassesHeap = 0; ///Quantidade de classes na memoria
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.
    u1 * full_file_name = (u1*)malloc(sizeof(u1)*MAX_CLASSES_ON_HEAP);

    ///Trata o caso de passar o PATH completo
    strcpy(full_file_name,argv[1]);
    if(argc > 2){
        for(int i = 2; i < argc-1;i++ ){ ///Necessaro pois para cada espaço em branco no PATH eh criado uma nova string.
            strcat(full_file_name," ");
            strcat(full_file_name,argv[i]);
            //if(i < argc -2)
              //  strcat(full_file_name," ");
        }

        if(strcmp(argv[argc - 1],"-print")){
            strcat(full_file_name," ");
            strcat(full_file_name,argv[argc - 1]);
        }
    }

//    printf("PATH + NAME: %s\n\n\n\n",full_file_name);

    ///Carrega a classe informada por linha de comando no HEAP
    //classLoader(class_file+numberOfClassesHeap, *argv, &numberOfClassesHeap);
    classLoader(class_file+numberOfClassesHeap, full_file_name, &numberOfClassesHeap);
    //classPrint(class_file+numberOfClassesHeap-1);

    if(argc > 2 && !strcmp(argv[argc - 1],"-print")) ///Verifica se a flag para printar o .class foi passado pela linha de comando.
        classPrint(class_file+numberOfClassesHeap-1);

    ///Carrega o vetor com o incremento de PC para cada instrucao
    fillNumberOfByteInstruction(&numberOfByteInstruction);

    ///Salva na memoria o PATH do arquivo de entrada.
    getRootDirectory(full_file_name);
    //printf("PATH: %s\n",rootDirectory);

    ///Instancia um novo Objeto.
    createNewObject(handler,&numberOfClasses,class_file+numberOfClassesHeap-1, frameStackTop,&numberOfByteInstruction);
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
//printf("%p\n", class_file);
    ///Chama a funcao responsavel por efetivamente rodar a JVM
    runJVM(cur_frame,&curPC,&numberOfByteInstruction, frameStackTop, class_file, &numberOfClassesHeap, &numberOfClasses, handler);

    return 0;
}


void runJVM(Frame * cur_frame,u4 * curPC, u1 * numberOfByteInstruction, StructFrameStack *frameStackTop, ClassFile * class_file, u4 * numberOfClassesHeap, u4 * numberOfClasses, ClassHandler * handler){
    u1 curOPCODE, flagIsWide = 0;
    //cur_frame->operandStack->next = NULL;
    u4 attributeCodeIndex = findAttributeCodeIndex(cur_frame->methods->attributes,cur_frame->methods->attributes_count); ///Localiza o indice do attriute Code
    while(cur_frame->methods->attributes[attributeCodeIndex].Code.code[*curPC] && *curPC != NOT_RETURN){ ///Enquanto o code nao for NULL, repete
        //printf("\tMETODO ATUAL: %s\n",cur_frame->handler->classRef->constant_pool[cur_frame->methods->name_index].UTF8.bytes);
        curOPCODE = getOpcode(&cur_frame->methods->attributes[attributeCodeIndex], *curPC); ///Procura pelo OPCODE apontado por curPC.
        //printf("\tOPCODE: %d\tPC: %d\n", curOPCODE, *curPC);
        if(curOPCODE == OPCODE_wide){
            flagIsWide = 1; ///Seta a flag avisando que a proxima instrucao eh do tipo WIDE.
            ///Incrementa PC
            incPC(curPC,curOPCODE,numberOfByteInstruction);
        }else{
            ///Chama a funcao que realiza as instrucoes. Esta sendo implementada pelo GVT.

            if((curOPCODE > 152 && curOPCODE < 178) || (curOPCODE > 197 && curOPCODE < 202)){///Instrucoes com deslocamento
                doInstructionShift(&cur_frame, curPC, frameStackTop, cur_frame->methods->attributes[attributeCodeIndex].Code.code, flagIsWide);
            }else if(curOPCODE > 181 && curOPCODE < 188){
                doInstructionInvoke(cur_frame,frameStackTop, handler, curPC,flagIsWide,cur_frame->methods->attributes[attributeCodeIndex].Code.code, class_file,numberOfClassesHeap, numberOfClasses,numberOfByteInstruction);
                //u4 attributeCodeIndex = findAttributeCodeIndex(cur_frame->methods->attributes,cur_frame->methods->attributes_count); ///Localiza o indice do attriute Code
                ///Atualiza PC para o novo Frame.
                if(curOPCODE != OPCODE_invokestatic && curOPCODE != OPCODE_invokespecial){
                    incPC(curPC,curOPCODE,numberOfByteInstruction);
                }
            }else if((curOPCODE>45 && curOPCODE<54)||(curOPCODE>78 && curOPCODE<87) || (curOPCODE>187 && curOPCODE<191) || curOPCODE==197){ ///Instrucoes referentes aos arrays
                doInstructionArray(cur_frame, *curPC, flagIsWide, cur_frame->methods->attributes[attributeCodeIndex].Code.code);
                incPC(curPC,curOPCODE,numberOfByteInstruction);
                //printf("entrou doInstructionArray\n" );
            }
            else{
                doInstruction(cur_frame, *curPC, flagIsWide, cur_frame->methods->attributes[attributeCodeIndex].Code.code);
                //printf("entrou doInstruction\n" );
                ///Incrementa PC
                incPC(curPC,curOPCODE,numberOfByteInstruction);
            }
            flagIsWide = 0; ///Zera a flag de instrucao WIDE.
//            printStack((cur_frame)->operandStack);///DEBUGGER---------------------------------------------------------------
            //getchar();
        }
        if(*curPC == -1){ ///Programa encerrado. return da main chamado
            break;
        }
        ///Incrementa PC
        ///incPC(curPC,curOPCODE,numberOfByteInstruction);
        //system("pause");
    }
    //printf("saiu\n");
    //printf("PC: %d\n",*curPC);
}

void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file, StructFrameStack *frameStackTop,u1 * numberOfByteInstruction){
    u4 aux = *numberOfClasses +1;
    //printf("TESTE 1: %d\n",aux);
    //handler = (ClassHandler *)realloc(handler,aux * sizeof(ClassHandler));///Inicializa o Objeto
    //printf("TESTE 2!\n");
    if(handler == NULL)
        throwException(INSUFFICIENT_MEMORY,INSUFFICIENT_MEMORY_MSG);///Verifica se foi possivel alocar memoria suficiente
    newObject(handler+(*numberOfClasses),class_file); ///Instancia um novo Objeto da classe class_file
    //printf("TESTE 3!\n");
    //printHandler(handler+(*numberOfClasses));
    *numberOfClasses = *numberOfClasses + 1; ///Atualiza a quantidade de classes instanciadas.

    ///Verifica a existencia do metodo <clinit>
    u1 * clinit = "<clinit>";
    u4 clinit_method_index = seekNewMethodInClassFile(class_file,clinit,"()V"); ///Procura pelo <clinit>
    u4 attributeCodeIndex = findAttributeCodeIndex(class_file->methods->attributes,class_file->methods->attributes_count);
    if(clinit_method_index != NOT_RETURN){ ///Caso o metodo <<clinit>> exita.
        createNewFrame(handler, clinit_method_index, 0,frameStackTop); ///Cria um novo frame pro <<clinit>>
        Frame * clinit_frame = (Frame *)malloc(sizeof(Frame));
        clinit_frame = popFrameStack(frameStackTop);
        ///Executa as operacoes do <clinit>
        doCLInit(class_file,class_file->methods->attributes[attributeCodeIndex].Code.code,clinit_frame,numberOfByteInstruction);
    }
}

void createNewFrame(ClassHandler * handler, u4 method_index, u4 curPC,StructFrameStack *frameStackTop){
    //Frame * newFrame = (Frame *)malloc(sizeof(Frame));
    Frame * frame = (Frame *)malloc(sizeof(Frame));
    newFrame(frame,handler,method_index,curPC);///Cria o novo metodo de indice method_index referente ao Objeto apontado por handler
    pushFrameStack(frameStackTop, frame);///Coloca o frame no topo da pilha de frames.
}

void pushFrameStack(StructFrameStack *frameStackTop, Frame * frame){
//    StructFrameStack *nodeFrame;
//    nodeFrame = (StructFrameStack*)malloc(sizeof(StructFrameStack));
//    nodeFrame->frame = frame;
//    nodeFrame->next = *frameStackTop;
//    *frameStackTop = nodeFrame;
    if (frameStackTop->stackTop == FRAME_STACK_MAX-1) {
		printf(" Erro: Pilha sem memoria.\n");
		exit(1);
	}
	frameStackTop->frame[++(frameStackTop->stackTop)] = *frame;
//printf("Push Frame: %s\n",frameStackTop->frame[frameStackTop->stackTop].constant_pool[frameStackTop->frame[frameStackTop->stackTop].methods->name_index].UTF8.bytes);
//    StructFrameStack **aux_node = &frameStackTop;
//    StructFrameStack *nodeFrame = NULL;
//    nodeFrame = (StructFrameStack*)malloc(sizeof(StructFrameStack));///Instancia novo membro da pilha de frames
//    nodeFrame->frame = frame;///Salva o frame no novo membro da pilha.
//    //printf("\tPush1: frameStackTop: %d\tnodeFrame: %d\tNext: %d\n",*frameStackTop,*nodeFrame,nodeFrame->next);
//    //(*nodeFrame).next = &frameStackTop; ///Novo membro da pilha aponta para o topo da pilha corrente
//    nodeFrame->next = *aux_node; ///Novo membro da pilha aponta para o topo da pilha corrente
//    //frameStackTop = &nodeFrame;///Novo membro vira o topo da pilha.
//    *aux_node = nodeFrame;///Novo membro vira o topo da pilha.
//    //printf("\tPush2: frameStackTop: %d\tnodeFrame: %d\tNext: %d\n",*frameStackTop,*nodeFrame,nodeFrame->next);
//    printf("Push Frame: %s\n",frameStackTop->frame->constant_pool[frameStackTop->frame->methods->name_index].UTF8.bytes);
}

Frame * popFrameStack(StructFrameStack *frameStackTop){
//StructFrameStack * aux;
//    Frame * currentframe;
//    aux = *frameStackTop;
//    currentframe = (*frameStackTop)->frame;
//    //*frameStackTop = (*frameStackTop)->next;
//
//    if(!(*frameStackTop)->next){
//            printf("entrou no IF\n");
//        *frameStackTop = (StructFrameStack*)malloc(sizeof(StructFrameStack));
//    }else{
//        printf("entrou no ELSE\n");
//        *frameStackTop = (*frameStackTop)->next;
//        free(aux);
//    }
//    printf("Pop Frame: %s\n",currentframe->constant_pool[currentframe->methods->name_index].UTF8.bytes);
//    return currentframe;

if (frameStackTop->stackTop < 0) {
		printf(" Erro: Acesso a posicao invalida da pilha.\n\n");
		exit(1);
	}
	Frame * currentframe = (Frame*)malloc(sizeof(Frame));///Instancia um novo frame
	*currentframe = frameStackTop->frame[(frameStackTop->stackTop)];
	//frameStackTop->frame[(frameStackTop->stackTop)] = (Frame *)malloc(sizeof(Frame));;
//	printf("Pop Frame: %s\n",frameStackTop->frame[frameStackTop->stackTop].constant_pool[frameStackTop->frame[frameStackTop->stackTop].methods->name_index].UTF8.bytes);
	frameStackTop->stackTop--;
	return currentframe;
	//return frameStackTop->frame[(frameStackTop->stackTop)--];

/*
    if(frameStackTop != NULL){
        //StructFrameStack ** aux2 = &frameStackTop;
        StructFrameStack * aux = (StructFrameStack*)malloc(sizeof(StructFrameStack));
        //StructFrameStack * aux = *aux2;
        Frame * currentframe = NULL;
        currentframe = (Frame*)malloc(sizeof(Frame));///Instancia um novo frame
        currentframe = frameStackTop->frame;///Novo frame recebe o frame salvo no topo da pilha
        //printf("TETE: %s\n",(*aux2)->frame->constant_pool[currentframe->methods->name_index].UTF8.bytes);
        //currentframe = (*aux2)->frame;///Novo frame recebe o frame salvo no topo da pilha
        //aux = frameStackTop;
        if(frameStackTop->next){
                printf("\tAntes => Pop Frame: %s\n",frameStackTop->frame->constant_pool[frameStackTop->frame->methods->name_index].UTF8.bytes);
                //*aux2 = (*aux2)->next;
            *frameStackTop = *frameStackTop->next;///Novo topo da fila vira o membro apontado pelo frame retirado.
                printf("\tDepois => Pop Frame: %s\n",frameStackTop->frame->constant_pool[frameStackTop->frame->methods->name_index].UTF8.bytes);
            //free(aux);
        }else{
            printf("Entrei no ELSE!\n");
            StructFrameStack ** aux2 = &frameStackTop;
            (*aux2)->next = NULL;
            *aux2 = (StructFrameStack*)malloc(sizeof(StructFrameStack));
            //free(frameStackTop);
            //free(*aux2);
        }
        //free(aux);
        printf("Pop Frame: %s\n",currentframe->constant_pool[currentframe->methods->name_index].UTF8.bytes);
        return currentframe;
    }else{
        char stackName = "Pilha de Frames";
        throwException(POP_IN_A_EMPTY_STACK,POP_IN_A_EMPTY_STACK_MSG,stackName);
    }
    */
}

int createMainFrame(ClassHandler * handler,u4 curPC,StructFrameStack *frameStackTop){
    for(int i = 0; i < handler->classRef->methods_count; i++){
        if(!strcmp(handler->classRef->constant_pool[ handler->classRef->methods[i].name_index ].UTF8.bytes,"main")){ ///Procura pelo metodo main no method_info da classe
            //printf("Achei a MAIN no indice %d\n",i);
            createNewFrame(handler,i,NOT_RETURN,frameStackTop); ///encontrou o metodo main no indice i, cria o frame dele.
//            frameStackTop->next = NULL;
            return MAIN_FOUND; ///Retorna sucesso
        }
    }
    throwException(MAIN_NOT_FOUND,MAIN_NOT_FOUND_MSG); ///Nao encontrou a main, taca erro.
}

void getRootDirectory(u1 * file_name){
    int i;
    int fileNameLength = strlen(file_name);
    char tmp_file_name[fileNameLength];
    strcpy(tmp_file_name,file_name);

    if(strchr(tmp_file_name,'/') || strchr(tmp_file_name,'\\')){ ///Verifica se existe o caracter '/' no nome do arquivo
        for(i = fileNameLength; i >= 0; i--){
            if(tmp_file_name[i] == '/' || tmp_file_name[i] == '\\'){ ///Procura a posicao do ultimo '/' ou '\' no nome do arquivo
                rootDirectory = (u1 *)malloc((i+1) * sizeof(u1));
                for(int j = 0; j <= i; j++ ){
                    rootDirectory[j] = tmp_file_name[j]; ///Copia os caracteres do PATH para a variavel global
                }
                rootDirectory[i+1] = '\0'; ///Insere o caractere de fimd e string.
                break;
            }
        }
    }else{
        rootDirectory = NULL; ///O diretorio raiz eh o mesmo do executavel.
    }
}
