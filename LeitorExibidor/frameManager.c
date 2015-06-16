#define FRAMEMANAGER_SERV
#include "lib/frameManager.h"
#include <stdio.h>
#include <stdlib.h>

void newObject(ClassHandler * handler, ClassFile * class_file){
    //handler = (ClassHandler *)malloc(sizeof(ClassHandler));
    handler->classRef = class_file;

    handler->fields = (field_info *)malloc(handler->classRef->fields_count * sizeof(field_info));

    for(int i = 0; i < handler->classRef->fields_count; i++){
        handler->fields[i].access_flags = handler->classRef->fields[i].access_flags;
        handler->fields[i].name_index = handler->classRef->fields[i].name_index;
        handler->fields[i].descriptor_index = handler->classRef->fields[i].descriptor_index;
        handler->fields[i].attributes_count = handler->classRef->fields[i].attributes_count;

        handler->fields[i].attributes = (attribute_info *)malloc(handler->fields[i].attributes_count * sizeof(attribute_info));
        copyAttributesInfo(handler->fields[i].attributes,handler->fields[i].attributes_count, handler->classRef->fields[i].attributes, handler->classRef->constant_pool); ///Chama a função para copiar as informações dos atributos
        //printAttributesInfo(handler->fields[i].attributes, handler->fields[i].attributes_count, handler->classRef->constant_pool, handler->classRef);
    }/*
    printFields(handler->classRef->fields_count, handler->fields);
    for(int i = 0; i < handler->classRef->fields_count; i++){
        printAttributesInfo(handler->fields[i].attributes, handler->fields[i].attributes_count, handler->classRef->constant_pool, handler->classRef);
    }*/
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

void pushOperandStack(structOperandStack **operandStackTop, u4 operand){
    structOperandStack *nodeOperand;
    nodeOperand = (structOperandStack*)malloc(sizeof(structOperandStack));
    nodeOperand->value = operand;
    nodeOperand->next = *operandStackTop;
    *operandStackTop = nodeOperand;
}

u4 popOperandStack(structOperandStack **operandStackTop){
    structOperandStack *aux;
    u4 operand;
    aux = *operandStackTop;
    operand = aux->value;
    *operandStackTop = (*operandStackTop)->next;
    free(aux);
    return operand;
}
