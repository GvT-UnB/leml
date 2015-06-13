#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"
#include "lib/UserViewer.h"
//#include "lib/frameManager.h"

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


/** \brief Copia o conteudo de attributesSrc para attributes
 *
 * \param attributes attribute_info* Vetor destino de attribute_info
 * \param attributes_count u2 Tamanho do vetor
 * \param attributesSrc attribute_info* Vetor fonte de attribute_info
 * \param constant_pool cp_info * referencia para o constant pool da classe
 * \return void
 *
 */
void copyAttributesInfo(attribute_info * attributes, u2 attributes_count, attribute_info * attributesSrc, cp_info * constant_pool);

/** \brief Copia o conteudo de classesSrc para classesDst
 *
 * \param classesDst Classes* Vetor destino de Classes
 * \param number_of_classes u2 Tamanho dos vetores
 * \param classesSrc Classes* Vetor fonte de Classes
 * \return void
 *
 */
void copyClasses(Classes * classesDst, u2 number_of_classes, Classes * classesSrc);

/** \brief Copia o conteudo de line_number_table_src para line_number_table_dst
 *
 * \param line_number_table_dst Line_number_table*  Vetor destino de Line Number Table
 * \param line_number_table_length u2 Tamanho do vetor
 * \param line_number_table_src Line_number_table* Vetor fonte de Line Number Table
 * \return void
 *
 */
void copyLineNumberTable(Line_number_table * line_number_table_dst, u2 line_number_table_length, Line_number_table * line_number_table_src);

/** \brief Copia o conteudo de local_variable_table_src para local_variable_table_dst
 *
 * \param local_variable_table_dst Local_variable_table* Vetor destino de Local Variable Table
 * \param local_variable_table_length u2 Tamanho do vetor
 * \param local_variable_table_src Local_variable_table* Vetor fonte de Local Variable Table
 * \return void
 *
 */
void copyLocalVariableTable(Local_variable_table * local_variable_table_dst, u2 local_variable_table_length, Local_variable_table * local_variable_table_src);

int main(int argc, char *argv[]){
    FILE * dot_class;
    ClassFile * class_file;
    //char *program_full_name = argv[0]; /* nome do programa para caso de erro */
    u4 PC;
    u1 numberOfByteInstruction[MAX_INSTRUCTIONS]; ///Vetor que armazena a quantidade de bytes que cada instrução utiliza.

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

    ClassHandler * handler = (ClassHandler *)malloc(sizeof(ClassHandler *));
    handler->classRef = class_file;
    //classPrint(handler->classRef);
    handler->fields = (field_info *)malloc(handler->classRef->fields_count * sizeof(field_info *));

    for(int i = 0; i < handler->classRef->fields_count; i++){
        handler->fields[i].access_flags = handler->classRef->fields[i].access_flags;
        //printf("Access Flags:\t\t0x%04x\n",handler->fields[i].access_flags);
        handler->fields[i].name_index = handler->classRef->fields[i].name_index;
        //printf("Name:\t\tcp_info #%d\n", handler->fields[i].name_index);
        handler->fields[i].descriptor_index = handler->classRef->fields[i].descriptor_index;
        //printf("Descriptor:\tcp_info #%d\n", handler->fields[i].descriptor_index);
        handler->fields[i].attributes_count = handler->classRef->fields[i].attributes_count;
        //printf("Attribute count:\t\t%d\n\n", handler->fields[i].attributes_count);

        handler->fields[i].attributes = (attribute_info *)malloc(handler->fields[i].attributes_count * sizeof(attribute_info *));
        copyAttributesInfo(handler->fields[i].attributes,handler->fields[i].attributes_count, handler->classRef->fields[i].attributes, handler->classRef->constant_pool); ///Chama a função para copiar as informações dos atributos
        printAttributesInfo(handler->fields[i].attributes, handler->fields[i].attributes_count, handler->classRef->constant_pool, handler->classRef);

        printf("================================================================\n");
        printf("================================================================\n");
        //system("pause");
        //getchar();
    }
/*
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printFields(handler->classRef->fields_count, handler->fields);
    for(int i = 0; i < handler->classRef->fields_count; i++){
        printAttributesInfo(handler->fields[i].attributes, handler->fields[i].attributes_count, handler->classRef->constant_pool, handler->classRef);
    }
*/

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



void copyAttributesInfo(attribute_info * attributes, u2 attributes_count, attribute_info * attributesSrc, cp_info * constant_pool){
    char *indice = NULL;
    for(int j = 0; j < attributes_count; j++){
        attributes[j].name_index = attributesSrc[j].name_index;
        attributes[j].length = attributesSrc[j].length;
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
                attributes[j].ConstantValue.index = attributesSrc[j].ConstantValue.index;
                break;
            case ATTRIBUTE_Code:
                attributes[j].Code.max_stack = attributesSrc[j].Code.max_stack;
                attributes[j].Code.max_locals = attributesSrc[j].Code.max_locals;
                attributes[j].Code.code_length = attributesSrc[j].Code.code_length;
                attributes[j].Code.code = (u1 *)malloc((attributes[j].Code.code_length * sizeof(u1))+1);
                for(int k =0; k < attributes[j].Code.code_length; k++){
                    attributes[j].Code.code[k] = attributesSrc[j].Code.code[k];
                }
                attributes[j].Code.code[attributes[j].Code.code_length] =  '\0';
                attributes[j].Code.exception_table_length = attributesSrc[j].Code.exception_table_length;
                attributes[j].Code.ExceptionTable = (ExceptionTable *)malloc(attributes[j].Code.exception_table_length * sizeof(ExceptionTable));
                for(int k =0; k < attributes[j].Code.exception_table_length; k++){
                    attributes[j].Code.ExceptionTable[k].start_pc = attributesSrc[j].Code.ExceptionTable[k].start_pc;
                    attributes[j].Code.ExceptionTable[k].end_pc = attributesSrc[j].Code.ExceptionTable[k].end_pc;
                    attributes[j].Code.ExceptionTable[k].handler_pc = attributesSrc[j].Code.ExceptionTable[k].handler_pc;
                    attributes[j].Code.ExceptionTable[k].catch_type = attributesSrc[j].Code.ExceptionTable[k].catch_type;
                }
                attributes[j].Code.attributes_count = attributesSrc[j].Code.attributes_count;
                attributes[j].Code.attributes = (attribute_info *)malloc(attributes[j].Code.attributes_count * sizeof(attribute_info));
                copyAttributesInfo(attributes[j].Code.attributes,attributes[j].Code.attributes_count,attributesSrc[j].Code.attributes,constant_pool);
                break;
            case ATTRIBUTE_Exceptions:
                attributes[j].Exceptions.number_of_exceptions = attributesSrc[j].Exceptions.number_of_exceptions;
                attributes[j].Exceptions.exception_index_table = (u2 *)malloc(attributes[j].Exceptions.number_of_exceptions * sizeof(u2));
                for(int k =0; k < attributes[j].Exceptions.number_of_exceptions; k++){
                    attributes[j].Exceptions.exception_index_table[k] = attributesSrc[j].Exceptions.exception_index_table[k];
                }
                break;
            case ATTRIBUTE_InnerClasses:
                attributes[j].InnerClasses.number_of_classes = attributesSrc[j].InnerClasses.number_of_classes;
                attributes[j].InnerClasses.Classes = (Classes *)malloc(attributes[j].InnerClasses.number_of_classes * sizeof(Classes));
                copyClasses(attributes[j].InnerClasses.Classes, attributes[j].InnerClasses.number_of_classes, attributesSrc[j].InnerClasses.Classes);
                break;
            case ATTRIBUTE_Syntethic:
                break;
            case ATTRIBUTE_LineNumber:
                attributes[j].LineNumber.line_number_table_length = attributesSrc[j].LineNumber.line_number_table_length;
                attributes[j].LineNumber.Line_number_table = (Line_number_table *)malloc(attributes[j].LineNumber.line_number_table_length * sizeof(Line_number_table));
                copyLineNumberTable(attributes[j].LineNumber.Line_number_table, attributes[j].LineNumber.line_number_table_length, attributesSrc[j].LineNumber.Line_number_table);
                break;
            case ATTRIBUTE_LocalVariable:
                attributes[j].LocalVariable.local_variable_table_length = attributesSrc[j].LocalVariable.local_variable_table_length;
                attributes[j].LocalVariable.Local_variable_table = (Local_variable_table *)malloc(attributes[j].LocalVariable.local_variable_table_length * sizeof(Local_variable_table));
                copyLocalVariableTable(attributes[j].LocalVariable.Local_variable_table,attributes[j].LocalVariable.local_variable_table_length,attributesSrc[j].LocalVariable.Local_variable_table);
                break;
            case ATTRIBUTE_SourceFile:
                attributes[j].SourceFile.sourcefile_index = attributesSrc[j].SourceFile.sourcefile_index;
                break;
            default: //ATTRIBUTE_Unknown
                break;
        }
    }
}

void copyClasses(Classes * classesDst, u2 number_of_classes, Classes * classesSrc){
    for(int i = 0; i < number_of_classes; i++){
        classesDst[i].inner_class_info_index = classesSrc[i].inner_class_info_index;
        classesDst[i].outer_class_info_index = classesSrc[i].outer_class_info_index;
        classesDst[i].inner_name_index = classesSrc[i].inner_name_index;
        classesDst[i].inner_class_access_flags = classesSrc[i].inner_class_access_flags;
    }
}

void copyLineNumberTable(Line_number_table * line_number_table_dst, u2 line_number_table_length, Line_number_table * line_number_table_src){
    for(int i = 0; i < line_number_table_length; i++){
        line_number_table_dst[i].start_pc = line_number_table_src[i].start_pc;
        line_number_table_dst[i].line_number = line_number_table_src[i].line_number;
    }
}

void copyLocalVariableTable(Local_variable_table * local_variable_table_dst, u2 local_variable_table_length, Local_variable_table * local_variable_table_src){
    for(int i = 0; i < local_variable_table_length; i++){
        local_variable_table_dst[i].start_pc = local_variable_table_src[i].start_pc;
        local_variable_table_dst[i].length = local_variable_table_src[i].length;
        local_variable_table_dst[i].name_index = local_variable_table_src[i].name_index;
        local_variable_table_dst[i].descriptor_index = local_variable_table_src[i].descriptor_index;
        local_variable_table_dst[i].index = local_variable_table_src[i].index;
    }
}
