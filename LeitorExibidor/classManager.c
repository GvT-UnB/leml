#define CLASSMANAGER_SERV
#include <stdio.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"


ClassFile * classRead(FILE * dot_class){
    ClassFile * class_file;

    class_file = (ClassFile *)malloc(sizeof(ClassFile)); //Aloca espaço na memória para o arquivo .class

    class_file->magic = u4Read(dot_class);
    if(class_file->magic != U4_MAGIC) //Verifica se o arquivo informado eh um bytecode java
        throwException(NOT_BYTECODE_JAVA, NOT_BYTECODE_JAVA_MSG);

    class_file->minor_version = u2Read(dot_class);
    class_file->major_version = u2Read(dot_class);
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



