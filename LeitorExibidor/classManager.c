#define CLASSMANAGER_SERV
#include <stdio.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/instruction.h"


ClassFile * classRead(FILE * dot_class,ClassFile * class_file){
    //ClassFile * class_file;

    //class_file = (ClassFile *)malloc(sizeof(ClassFile)); //Aloca espaço na memória para o arquivo .class

    class_file->magic = u4Read(dot_class);
    if(class_file->magic != U4_MAGIC) //Verifica se o arquivo informado eh um bytecode java
        throwException(NOT_BYTECODE_JAVA, NOT_BYTECODE_JAVA_MSG);

    class_file->minor_version = u2Read(dot_class);
    class_file->major_version = u2Read(dot_class);
    if(class_file->major_version > 48) //Verifica se o bytecode informado é compativel com Java 2 ou inferior.
        throwException(NOT_JAVA_2_VERSION,NOT_JAVA_2_VERSION_MSG);
    class_file->constant_pool_count = u2Read(dot_class);
    class_file->constant_pool = (cp_info *)malloc(class_file->constant_pool_count * sizeof(cp_info)); //aloca espaço para o constant_pool


    for(int i = 1; i < class_file->constant_pool_count; i++){ //Lembre-se, o constant_pool começa em 1 !!!
        class_file->constant_pool[i].tag = u1Read(dot_class);
        //printf("%d\n", class_file->constant_pool[i].tag);
        switch(class_file->constant_pool[i].tag){ //Dependendo do valor da TAG, será utilizado uma diferente struct da Union do Constant Pool
            case CONSTANT_Class:
                class_file->constant_pool[i].Class.name_index = u2Read(dot_class);
                break;
            case CONSTANT_Fieldref:
                class_file->constant_pool[i].Fieldref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].Fieldref.name_and_type_index = u2Read(dot_class);
                break;
            case CONSTANT_Methodref:
                class_file->constant_pool[i].Methodref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].Methodref.name_and_type_index = u2Read(dot_class);
                break;
            case CONSTANT_InterfaceMethodref:
                class_file->constant_pool[i].InterfaceMethodref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].InterfaceMethodref.name_and_type_index = u2Read(dot_class);
                break;
            case CONSTANT_String:
                class_file->constant_pool[i].String.string_index = u2Read(dot_class);
                break;
            case CONSTANT_Integer:
                class_file->constant_pool[i].Integer.bytes = u4Read(dot_class);
                break;
            case CONSTANT_Float:
                class_file->constant_pool[i].Float.bytes = u4Read(dot_class);
                break;
            case CONSTANT_Long:
                class_file->constant_pool[i].Long.high_bytes = u4Read(dot_class);
                class_file->constant_pool[i].Long.low_bytes = u4Read(dot_class);
                i++;
                break;
            case CONSTANT_Double:
                class_file->constant_pool[i].Double.high_bytes = u4Read(dot_class);
                class_file->constant_pool[i].Double.low_bytes = u4Read(dot_class);
                i++;
                break;
            case CONSTANT_NameAndType:
                class_file->constant_pool[i].NameAndType.name_index = u2Read(dot_class);
                class_file->constant_pool[i].NameAndType.descriptor_index = u2Read(dot_class);
                break;
            case CONSTANT_Utf8:
                    class_file->constant_pool[i].UTF8.length = u2Read(dot_class);
                    class_file->constant_pool[i].UTF8.bytes = (u1 *)malloc((class_file->constant_pool[i].UTF8.length * sizeof(u1)) + 1);
                    for(int j = 0; j < class_file->constant_pool[i].UTF8.length; j++){
                        class_file->constant_pool[i].UTF8.bytes[j] = u1Read(dot_class);
                            //printf("\t\tBYTES: 0x%04x\n",class_file->constant_pool[i].UTF8.bytes[j]);
                    }
                    class_file->constant_pool[i].UTF8.bytes[class_file->constant_pool[i].UTF8.length] = '\0';
                break;
            default:
                break;
        }
        //system("pause");
    }

    class_file->access_flags = u2Read(dot_class);
    class_file->this_class = u2Read(dot_class);
    class_file->super_class = u2Read(dot_class); //Se for ZERO a classe estende a classe OBJECT
    class_file->interfaces_count = u2Read(dot_class);
    class_file->interfaces = (u2 *)malloc(class_file->interfaces_count * sizeof(u2));

    for(int i = 0; i < class_file->interfaces_count; i++){
        class_file->interfaces[i] = u2Read(dot_class);
    }

    class_file->fields_count = u2Read(dot_class);
    class_file->fields = (field_info *)malloc(class_file->fields_count * sizeof(field_info));
   //system("pause");

    for(int i = 0; i < class_file->fields_count; i++){
        class_file->fields[i].access_flags = u2Read(dot_class);
        class_file->fields[i].name_index = u2Read(dot_class);
        class_file->fields[i].descriptor_index = u2Read(dot_class);
        class_file->fields[i].attributes_count = u2Read(dot_class);

        class_file->fields[i].attributes = (attribute_info *)malloc(class_file->fields[i].attributes_count * sizeof(attribute_info));
        readAttributesInfo(class_file->fields[i].attributes,class_file->fields[i].attributes_count,dot_class, class_file->constant_pool); //Chama a função que carrega as informações dos atributos

    }

    //system("pause");
    class_file->methods_count = u2Read(dot_class);
    class_file->methods = (method_info *)malloc(class_file->methods_count * sizeof(method_info));

    for(int i = 0; i < class_file->methods_count; i++){
        class_file->methods[i].access_flags = u2Read(dot_class);
        class_file->methods[i].name_index = u2Read(dot_class);
        class_file->methods[i].descriptor_index = u2Read(dot_class);
        class_file->methods[i].attributes_count = u2Read(dot_class);
        class_file->methods[i].attributes = (attribute_info *)malloc(class_file->methods[i].attributes_count * sizeof(attribute_info));
        readAttributesInfo(class_file->methods[i].attributes,class_file->methods[i].attributes_count,dot_class, class_file->constant_pool); //Chama a função que carrega as informações dos atributos
    }
    //system("pause");

    class_file->attributes_count = u2Read(dot_class);
    class_file->attributes = (attribute_info *)malloc(class_file->attributes_count * sizeof(attribute_info));
    readAttributesInfo(class_file->attributes,class_file->attributes_count,dot_class, class_file->constant_pool);


    return class_file;
}

void readAttributesInfo(attribute_info * attributes, u2 attributes_count, FILE * dot_class, cp_info * constant_pool){
    char *indice = NULL;
    for(int j = 0; j < attributes_count; j++){
        attributes[j].name_index = u2Read(dot_class);
        attributes[j].length = u4Read(dot_class);
        indice = constant_pool[attributes[j].name_index].UTF8.bytes;

        if (strcmp("ConstantValue", indice) == 0){
            attributes[j].tag = ATTRIBUTE_ConstantValue;
        }
        else if (strcmp("Code", indice) == 0){
            attributes[j].tag = ATTRIBUTE_Code;
        }
        else if (strcmp("Exceptions", indice) == 0){
            attributes[j].tag = ATTRIBUTE_Exceptions;
        }
        else if (strcmp("InnerClasses", indice) == 0){
            attributes[j].tag = ATTRIBUTE_InnerClasses;
        }
        else if (strcmp("Synthetic", indice) == 0){
            attributes[j].tag = ATTRIBUTE_Syntethic;
        }
        else if (strcmp("LineNumberTable", indice) == 0){
            attributes[j].tag = ATTRIBUTE_LineNumber;
        }
        else if (strcmp("LocalVariableTable", indice) == 0){
            attributes[j].tag = ATTRIBUTE_LocalVariable;
        }
        else if (strcmp("SourceFile", indice) == 0){
            attributes[j].tag = ATTRIBUTE_SourceFile;
        }
        else{
            attributes[j].tag = ATTRIBUTE_Unknown;
        }
        //printf("\tTAG: %d\n",attributes[j].tag);
        switch(attributes[j].tag){
            case ATTRIBUTE_ConstantValue:
                attributes[j].ConstantValue.index = u2Read(dot_class);
                break;
            case ATTRIBUTE_Code:
                attributes[j].Code.max_stack = u2Read(dot_class);
                attributes[j].Code.max_locals = u2Read(dot_class);
                attributes[j].Code.code_length = u4Read(dot_class);
                attributes[j].Code.code = (u1 *)malloc((attributes[j].Code.code_length * sizeof(u1))+1);
                for(int k =0; k < attributes[j].Code.code_length; k++){
                    attributes[j].Code.code[k] =  u1Read(dot_class);
                }
                attributes[j].Code.code[attributes[j].Code.code_length] =  '\0';
                attributes[j].Code.exception_table_length = u2Read(dot_class);
                attributes[j].Code.ExceptionTable = (ExceptionTable *)malloc(attributes[j].Code.exception_table_length * sizeof(ExceptionTable));
                for(int k =0; k < attributes[j].Code.exception_table_length; k++){
                    attributes[j].Code.ExceptionTable[k].start_pc =  u2Read(dot_class);
                    attributes[j].Code.ExceptionTable[k].end_pc =  u2Read(dot_class);
                    attributes[j].Code.ExceptionTable[k].handler_pc =  u2Read(dot_class);
                    attributes[j].Code.ExceptionTable[k].catch_type =  u2Read(dot_class);
                }
                attributes[j].Code.attributes_count = u2Read(dot_class);
                attributes[j].Code.attributes = (attribute_info *)malloc(attributes[j].Code.attributes_count * sizeof(attribute_info));
                readAttributesInfo(attributes[j].Code.attributes,attributes[j].Code.attributes_count,dot_class, constant_pool);
                break;
            case ATTRIBUTE_Exceptions:
                attributes[j].Exceptions.number_of_exceptions = u2Read(dot_class);
                attributes[j].Exceptions.exception_index_table = (u2 *)malloc(attributes[j].Exceptions.number_of_exceptions * sizeof(u2));
                for(int k =0; k < attributes[j].Exceptions.number_of_exceptions; k++){
                    attributes[j].Exceptions.exception_index_table[k] =  u2Read(dot_class);
                }
                break;
            case ATTRIBUTE_InnerClasses:
                attributes[j].InnerClasses.number_of_classes = u2Read(dot_class);
                attributes[j].InnerClasses.Classes = (Classes *)malloc(attributes[j].InnerClasses.number_of_classes * sizeof(Classes));
                readClasses(attributes[j].InnerClasses.Classes,attributes[j].InnerClasses.number_of_classes,dot_class); //Chama a função responsavel por ler os dados das Classes
                break;
            case ATTRIBUTE_Syntethic:
                break;
            case ATTRIBUTE_LineNumber:
                attributes[j].LineNumber.line_number_table_length = u2Read(dot_class);
                attributes[j].LineNumber.Line_number_table = (Line_number_table *)malloc(attributes[j].LineNumber.line_number_table_length * sizeof(Line_number_table));
                readLineNumberTable(attributes[j].LineNumber.Line_number_table,attributes[j].LineNumber.line_number_table_length,dot_class);
                break;
            case ATTRIBUTE_LocalVariable:
                attributes[j].LocalVariable.local_variable_table_length = u2Read(dot_class);
                attributes[j].LocalVariable.Local_variable_table = (Local_variable_table *)malloc(attributes[j].LocalVariable.local_variable_table_length * sizeof(Local_variable_table));
                readLocalVariableTable(attributes[j].LocalVariable.Local_variable_table,attributes[j].LocalVariable.local_variable_table_length,dot_class);
                break;
            case ATTRIBUTE_SourceFile:
                attributes[j].SourceFile.sourcefile_index = u2Read(dot_class);
                break;
            default: //ATTRIBUTE_Unknown
                break;
        }
        //system("pause");
    }
}

void readClasses(Classes * classes, u2 number_of_classes, FILE * dot_class){
    for(int i = 0; i < number_of_classes; i++){
        classes[i].inner_class_info_index = u2Read(dot_class);
        classes[i].outer_class_info_index = u2Read(dot_class);
        classes[i].inner_name_index = u2Read(dot_class);
        classes[i].inner_class_access_flags = u2Read(dot_class);
    }
}

void readLineNumberTable(Line_number_table * line_number_table, u2 line_number_table_length, FILE * dot_class){
    for(int i = 0; i < line_number_table_length; i++){
        line_number_table[i].start_pc = u2Read(dot_class);
        line_number_table[i].line_number = u2Read(dot_class);
    }
}

void readLocalVariableTable(Local_variable_table * local_variable_table, u2 local_variable_table_length, FILE * dot_class){
    for(int i = 0; i < local_variable_table_length; i++){
        local_variable_table[i].start_pc = u2Read(dot_class);
        local_variable_table[i].length = u2Read(dot_class);
        local_variable_table[i].name_index = u2Read(dot_class);
        local_variable_table[i].descriptor_index = u2Read(dot_class);
        local_variable_table[i].index = u2Read(dot_class);
    }
}


u4 findAttributeCodeIndex(attribute_info * attributes, u2 attributes_count){
    for(int i = 0 ; i < attributes_count ; i++){ ///Percorre todo o attribute_info do metodo.
        if(attributes[i].tag == ATTRIBUTE_Code){ ///Procura pelo attribute_info CODE
            return i;
        }
    }
    return ATTRIBUTE_COD_NOT_FOUND;
}

void verifyClassName(char * argv, ClassFile * class_file){
    int i=0;
    int classNameLength = strlen(argv);
    char * className;
    char * classFullName = (char *)malloc(classNameLength * sizeof(char));
    classFullName = argv;

    ///Retira o caminho até o arquivo
    while(strchr(classFullName,'/')){
        while(classFullName[i] != '/'){
            i++;
        }

        className = (char *)malloc((classNameLength-i) * sizeof(char));
        i++;
        for(int j=0;j<=(classNameLength-i);j++){
            className[j] = classFullName[i+j];
        }
        classFullName = (char *)malloc(classNameLength * sizeof(char));
        strcpy(classFullName,className);
        classNameLength = strlen(classFullName);
    }

    ///Retira o .class do nome do arquivo
    i=0;
    while(classFullName[i] != '.'){
        i++;
    }
    free(className);
    className = (char *)malloc((i+1) * sizeof(char ));
    for(int j=0;j<i;j++){
        className[j] = classFullName[j];
    }
    className[i] = '\0';

    ///Verifica se o nomo do arquivo é o mesmo nome da classe
    //selectPointer(class_file, class_file->this_class, classRealName, 0);
    if(strcmp(className,getClassName(class_file))){
        throwException(CLASS_DIFFER_FILE_NAME,CLASS_DIFFER_FILE_NAME_MSG);
    }
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

u1 * getClassName(ClassFile * class_file){
    u2 this_class = class_file->this_class;
    u2 name_index = class_file->constant_pool[ this_class ].Class.name_index;
        //printf("%d\n",this_class);
        //printf("%d\n",name_index);
    return class_file->constant_pool[ name_index ].UTF8.bytes;
}

void setStaticFields(ClassFile * class_file){
    int j =0;
    char *fType, *fname;//tipo do field
    class_file->field_value = (Field_Value *)malloc(class_file->fields_count * sizeof(Field_Value));
    //frame->constant_pool[frame->constant_pool[class_file->fields[i].descriptor_index].NameAndType.name_index].UTF8.bytes;
    for(int i = 0; i < class_file->fields_count; i++){
        if(class_file->fields[i].access_flags & ACC_STATIC){
            fType = class_file->constant_pool[class_file->fields[i].descriptor_index].UTF8.bytes;
            class_file->field_value[j].field_index = i;
            fname = class_file->constant_pool[class_file->fields[i].name_index].UTF8.bytes;
//            printf("\nFIELD_NAME: %s\n",fname);
//            printf("FIELD_INDEX: %d\n", i);
            if((strstr(fType, "[") != NULL)||(strstr(fType, "L") != NULL)){
                class_file->field_value[j].U4.value = 0;
//                printf("Referencia\n");
            } else if((strstr(fType, "J") != NULL)||(strstr(fType, "D") != NULL)){
                class_file->field_value[j].U8.high = 0;
                class_file->field_value[j].U8.low = 0;
//                printf("double ou long\n");
            }else{
//                printf("u4\n");
                class_file->field_value[j].U4.value = 0;
            }
        }
    }
}

void classLoader(ClassFile * class_file, char * file_name, u4 * numberOfClassesHeap){
    FILE * dot_class;
    u1 * path = NULL;
//    printf("file_name: %s\n",file_name);
//    printf("rootDirectory: %s\n",rootDirectory);
    if((dot_class = fopen(file_name,"rb")) == NULL){ ///Verifica se o arquivo foi encontrado no diretorio raiz.
        path = (u1*)malloc(sizeof(u1)*(strlen(rootDirectory)+strlen(file_name)));
//        printf("Antes de tudo\n");
        strcpy(path,rootDirectory);
//        printf("PATH: %s\n",path);
        strcat(path,file_name);///Adiciona o PATH para o nome do arquivo.
//        printf("FULL PATH: %s\n",path);
        if((dot_class = fopen(path,"rb")) == NULL){
            throwException(OPEN_FILE_ERROR,OPEN_FILE_ERROR_MSG); ///Verifica se o arquivo foi encontrado
        }
    }
    //printf("Lendo o bytecode Java para a memoria...\n");
    *numberOfClassesHeap = *numberOfClassesHeap + 1;
    classRead(dot_class,class_file); ///Carrega para o HEAP a classe
    //classPrint(class_file);
    //printf("Bytecode Java copiado com sucesso!\n");
    fclose(dot_class);

    ///Verifica se foi preciso adicionar o PATH no nome do arquivo.
    if(path){
        class_file->class_full_name_length = strlen(path) - strlen(".class"); ///Deve-se retirar a string '.class' do nome completo da Classe.
        class_file->class_full_name = (u1*)malloc((class_file->class_full_name_length+1) * sizeof(u1));
        for(int i = 0; i < class_file->class_full_name_length; i++){
            class_file->class_full_name[i] = path[i]; ///Copia o nome completo da Classe, ignorando a parte do '.class'
        }
    }else{
        class_file->class_full_name_length = strlen(file_name) - strlen(".class"); ///Deve-se retirar a string '.class' do nome completo da Classe.
        class_file->class_full_name = (u1*)malloc((class_file->class_full_name_length+1) * sizeof(u1));
        for(int i = 0; i < class_file->class_full_name_length; i++){
            class_file->class_full_name[i] = file_name[i]; ///Copia o nome completo da Classe, ignorando a parte do '.class'
        }
    }
    class_file->class_full_name[class_file->class_full_name_length] = '\0'; ///Insere o caractere de fim de string.

//    printf("O nome completo da classe lida eh: %s\n",class_file->class_full_name);
    verifyClassName(file_name,class_file); ///Verifica se o .class tem o mesmo nome da classe delcarada nele.
    setStaticFields(class_file);
}


u4 seekClassInHeap(ClassFile * class_file,u4 numberOfClassesHeap,u1 * className){
    //printf("Devo procurar por: %s\n",className);
    for(u2 i = 0; i < numberOfClassesHeap; i++){
        //printf("Serah que eh %s?\n",(class_file+i)->class_full_name);
        if(!strcmp(className,(class_file+i)->class_full_name)){
            //printf("Encontrei a classe %s! Na posicao %d\n",getClassName(class_file+i),i);
            //printf("Encontrei a classe %s! Na posicao %d\n",(class_file+i)->class_full_name,i);
            return i;
        }
    }
    return CLASS_NOT_FOUND;
}

u4 loadNewClass(ClassFile * class_file,u4 * numberOfClassesHeap,u1 * className,ClassHandler * handler,u4 * numberOfClasses,StructFrameStack *frameStackTop,u1 * numberOfByteInstruction){
    u4 i;
    u1 * dotClass = ".class";

    ///Verifica se o className jah esta salvo no heap!
    i = seekClassInHeap(class_file,*numberOfClassesHeap,className);

    //printf("%s\n",className);
    ///Se a classe não estiver no HEAP, coloca no HEAP.
    if(i != CLASS_NOT_FOUND){
        //printf("Encontrei a classe %s! Na posicao %d\n",getClassName(class_file+i),i);
        return i; ///Se a classe jah esta na memoria, retorna o indice dela no HEAP.
    }else{
        //printf("Nao achei... :(\n");
        u1 * fullClassName = (u1*)malloc((strlen(className) + strlen(dotClass))*sizeof(u1));
        for(int i = 0; i < strlen(className);i++){
            fullClassName[i] = className[i];
        }
        for(int i = 0; i < strlen(dotClass);i++){
            fullClassName[i+strlen(className)] = dotClass[i]; ///Concatena o nome completo da classe com a string '.class'
        }
        //printf("Nome com extensao da classe: %s\n",fullClassName);
        classLoader(class_file+(*numberOfClassesHeap), fullClassName, numberOfClassesHeap);///Carrega a classe no HEAP.
        createNewObject(handler,numberOfClasses,class_file+(*numberOfClassesHeap)-1, frameStackTop,numberOfByteInstruction);///Instancia um Objeto da classe recem criada.
        //printf("Mas jah botei na memoria! ;D\n");
        return *numberOfClassesHeap; ///Retorna o indice da nova classe no HEAP.
    }
}

u4 seekNewMethodInClassHandler(ClassHandler * handler, u1 * method_name){
    for(int i = 0; i < handler->classRef->methods_count; i++){
        if(!strcmp(handler->classRef->constant_pool[ handler->classRef->methods[i].name_index ].UTF8.bytes, method_name)){ ///Procura pelo metodo no method_info da classe
            return i; ///Retorna o indice do metodo no method_info
        }
    }
    return NOT_RETURN; ///Nao encontrou o metodo na classe apontada pelo Frame
}

u4 seekNewMethodInClassFile(ClassFile * class_file, u1 * method_name){
    for(int i = 0; i < class_file->methods_count; i++){
        if(!strcmp(class_file->constant_pool[ class_file->methods[i].name_index ].UTF8.bytes, method_name)){ ///Procura pelo metodo no method_info da classe
            return i; ///Retorna o indice do metodo no method_info
        }
    }
    return NOT_RETURN; ///Nao encontrou o metodo na classe apontada pelo Frame
}
