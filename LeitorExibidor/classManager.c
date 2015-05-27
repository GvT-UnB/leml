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

    printf("Magic Number: 0x%08x\n",class_file->magic);
    printf("Minor Version: %d\n",class_file->minor_version);
    printf("Major Version: %d\n",class_file->major_version);
    printf("Constant Pool Counter: %d\n",class_file->constant_pool_count);


    for(int i = 1; i < class_file->constant_pool_count; i++){ //Lembre-se, o constant_pool começa em 1 !!!
        class_file->constant_pool->tag = u1Read(dot_class);
        printf("\nConstant Pool Tag: %d\n",class_file->constant_pool->tag);

        switch(class_file->constant_pool->tag){ //Dependendo do valor da TAG, será utilizado uma diferente struct da Union do Constant Pool
            case CONSTANT_Class:
                class_file->constant_pool->Class.name_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool->Class.name_index);
                break;
            case CONSTANT_Fieldref:
                class_file->constant_pool->Fieldref.class_index = u2Read(dot_class);
                class_file->constant_pool->Fieldref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->Fieldref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->Fieldref.name_and_type_index);
                break;
            case CONSTANT_Methodref:
                class_file->constant_pool->Methodref.class_index = u2Read(dot_class);
                class_file->constant_pool->Methodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->Methodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->Methodref.name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                class_file->constant_pool->InterfaceMethodref.class_index = u2Read(dot_class);
                class_file->constant_pool->InterfaceMethodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->InterfaceMethodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->InterfaceMethodref.name_and_type_index);
                break;
            case CONSTANT_String:
                class_file->constant_pool->String.string_index = u2Read(dot_class);
                    printf("\tSTRING_INDEX: %d\n",class_file->constant_pool->String.string_index);
                break;
            case CONSTANT_Integer:
                class_file->constant_pool->Integer.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool->Integer.bytes);
                break;
            case CONSTANT_Float:
                class_file->constant_pool->Float.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool->Float.bytes);
                break;
            case CONSTANT_Long:
                class_file->constant_pool->Long.high_bytes = u4Read(dot_class);
                class_file->constant_pool->Long.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool->Long.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool->Long.low_bytes);
                break;
            case CONSTANT_Double:
                class_file->constant_pool->Double.high_bytes = u4Read(dot_class);
                class_file->constant_pool->Double.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool->Double.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool->Double.low_bytes);
                break;
            case CONSTANT_NameAndType:
                class_file->constant_pool->NameAndType.name_index = u2Read(dot_class);
                class_file->constant_pool->NameAndType.descriptor_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool->NameAndType.name_index);
                    printf("\tDESCRIPTOR_INDEX: %d\n",class_file->constant_pool->NameAndType.descriptor_index);
                break;
            case CONSTANT_Utf8:
                    class_file->constant_pool->UTF8.length = u2Read(dot_class);
                        printf("\tLENGTH: %d\n",class_file->constant_pool->UTF8.length);
                        class_file->constant_pool->UTF8.bytes = (u1 *)malloc((class_file->constant_pool->UTF8.length * sizeof(u1)) + 1);
                    for(int j = 0; j < class_file->constant_pool->UTF8.length; j++){
                        class_file->constant_pool->UTF8.bytes[j] = u1Read(dot_class);
                            printf("\t\tBYTES: 0x%04x\n",class_file->constant_pool->UTF8.bytes[j]);
                    }
                    class_file->constant_pool->UTF8.bytes[class_file->constant_pool->UTF8.length] = '\0';
                    printf("\t\tBYTES CHAR: %s\n",class_file->constant_pool->UTF8.bytes);
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

    printf("ACCESS_FLAGS: 0x%04x\n",class_file->access_flags);
    printf("THIS_CLASS: %d\n",class_file->this_class);
    printf("SUPER_CLASS: %d\n",class_file->super_class);
    printf("Interfaces Counter: %d\n",class_file->interfaces_count);

    for(int i = 0; i < class_file->interfaces_count; i++){
        class_file->interfaces = u2Read(dot_class);
            printf("\tINTERFACES: %d\n",class_file->interfaces);
    }

    class_file->fields_count = u2Read(dot_class);
    printf("Fields Counter: %d\n",class_file->fields_count);
    class_file->fields = (field_info *)malloc(class_file->fields_count * sizeof(field_info));
   //system("pause");

    for(int i = 0; i < class_file->fields_count; i++){
        class_file->fields->access_flags = u2Read(dot_class);
        class_file->fields->name_index = u2Read(dot_class);
        class_file->fields->descriptor_index = u2Read(dot_class);
        class_file->fields->attributes_count = u2Read(dot_class);

        printf("\tACCESS_FLAGS: 0x%04x\n",class_file->fields->access_flags);
        printf("\tNAME_INDEX: 0x%04x\n",class_file->fields->name_index);
        printf("\tDESCRIPTOR_INDEX: 0x%04x\n",class_file->fields->descriptor_index);
        printf("\tATTRIBUTES_COUNT: 0x%04x\n",class_file->fields->attributes_count);
    }

    system("pause");
    class_file->methods_count = u2Read(dot_class);
    printf("Methods Counter: %d\n",class_file->methods_count);
    class_file->methods = (method_info *)malloc(class_file->methods_count * sizeof(method_info));

    for(int i = 0; i < class_file->methods_count; i++){
        class_file->methods->access_flags = u2Read(dot_class);
            printf("\tACCESS_FLAGS: 0x%04x\n",class_file->methods->access_flags);
        class_file->methods->name_index = u2Read(dot_class);
            printf("\tNAME_INDEX: 0x%04x\n",class_file->methods->name_index);
        class_file->methods->descriptor_index = u2Read(dot_class);
            printf("\tDESCRIPTOR_INDEX: 0x%04x\n",class_file->methods->descriptor_index);
        class_file->methods->attributes_count = u2Read(dot_class);
            printf("\tATTRIBUTES_COUNT: 0x%04x\n",class_file->methods->attributes_count);
        class_file->methods->attributes = (attribute_info *)malloc(class_file->methods->attributes_count * sizeof(attribute_info));

        readAttributesInfo(class_file->methods->attributes,class_file->methods->attributes_count,dot_class); //Chama a função que carrega as informações dos atributos
        printf("************************\n");
    }

    system("pause");
    class_file->attributes_count = u2Read(dot_class);
    printf("Attributes Counter: %d\n",class_file->attributes_count);
    class_file->attributes = (attribute_info *)malloc(class_file->attributes_count * sizeof(attribute_info));
    readAttributesInfo(class_file->attributes,class_file->attributes_count,dot_class);


    return class_file;
}

void readAttributesInfo(attribute_info * attributes, u2 attributes_count, FILE * dot_class){
    for(int j = 0; j < attributes_count; j++){
        attributes->name_index = u2Read(dot_class);
        attributes->length = u4Read(dot_class);
        attributes->tag = u1Read(dot_class);
        switch(attributes->tag){
            case ATTRIBUTE_ConstantValue:
                attributes->ConstantValue.index = u2Read(dot_class);
                break;
            case ATTRIBUTE_Code:
                attributes->Code.max_stack = u2Read(dot_class);
                attributes->Code.max_locals = u2Read(dot_class);
                attributes->Code.code_length = u4Read(dot_class);
                attributes->Code.code = (u1 *)malloc(attributes->Code.code_length * sizeof(u1));
                for(int k =0; k < attributes->Code.code_length; k++){
                    attributes->Code.code[k] =  u1Read(dot_class);
                }
                attributes->Code.exception_table_length = u2Read(dot_class);
                attributes->Code.ExceptionTable = (ExceptionTable *)malloc(attributes->Code.exception_table_length * sizeof(ExceptionTable));
                for(int k =0; k < attributes->Code.exception_table_length; k++){
                    attributes->Code.ExceptionTable[k].start_pc =  u2Read(dot_class);
                    attributes->Code.ExceptionTable[k].end_pc =  u2Read(dot_class);
                    attributes->Code.ExceptionTable[k].handler_pc =  u2Read(dot_class);
                    attributes->Code.ExceptionTable[k].catch_type =  u2Read(dot_class);
                }
                attributes->Code.attributes_count = u2Read(dot_class);
                attributes->Code.attributes = (attribute_info *)malloc(attributes->Code.attributes_count * sizeof(attribute_info));
                readAttributesInfo(attributes->Code.attributes,attributes->Code.attributes_count,dot_class);
                break;
            case ATTRIBUTE_Exceptions:
                attributes->Exceptions.number_of_exceptions = u2Read(dot_class);
                attributes->Exceptions.exception_index_table = (u2 *)malloc(attributes->Exceptions.number_of_exceptions * sizeof(u2));
                for(int k =0; k < attributes->Exceptions.number_of_exceptions; k++){
                    attributes->Exceptions.exception_index_table[k] =  u2Read(dot_class);
                }
                break;
            case ATTRIBUTE_InnerClasses:
                attributes->InnerClasses.number_of_classes = u2Read(dot_class);
                attributes->InnerClasses.Classes = (Classes *)malloc(attributes->InnerClasses.number_of_classes * sizeof(Classes));
                readClasses(attributes->InnerClasses.Classes,attributes->InnerClasses.number_of_classes,dot_class); //Chama a função responsavel por ler os dados das Classes
                break;
            case ATTRIBUTE_Syntethic:
                break;
            case ATTRIBUTE_LineNumber:
                attributes->LineNumber.line_number_table_length = u2Read(dot_class);
                attributes->LineNumber.Line_number_table = (Line_number_table *)malloc(attributes->LineNumber.line_number_table_length * sizeof(Line_number_table));
                readLineNumberTable(attributes->LineNumber.Line_number_table,attributes->LineNumber.line_number_table_length,dot_class);
                break;
            case ATTRIBUTE_LocalVariable:
                attributes->LocalVariable.local_variable_table_length = u2Read(dot_class);
                attributes->LocalVariable.Local_variable_table = (Local_variable_table *)malloc(attributes->LocalVariable.local_variable_table_length * sizeof(Local_variable_table));
                readLocalVariableTable(attributes->LocalVariable.Local_variable_table,attributes->LocalVariable.local_variable_table_length,dot_class);
                break;
            case ATTRIBUTE_SourceFile:
                attributes->SourceFile.sourcefile_index = u2Read(dot_class);
                break;
            case ATTRIBUTE_Unknown:
                break;
        }
    }
}

void readClasses(Classes * classes, u2 number_of_classes, FILE * dot_class){
    for(int i = 0; i < number_of_classes; i++){
        classes->inner_class_info_index = u2Read(dot_class);
        classes->outer_class_info_index = u2Read(dot_class);
        classes->inner_name_index = u2Read(dot_class);
        classes->inner_class_access_flags = u2Read(dot_class);
    }
}

void readLineNumberTable(Line_number_table * line_number_table, u2 line_number_table_length, FILE * dot_class){
    for(int i = 0; i < line_number_table_length; i++){
        line_number_table->start_pc = u2Read(dot_class);
        line_number_table->line_number = u2Read(dot_class);
    }
}

void readLocalVariableTable(Local_variable_table * local_variable_table, u2 local_variable_table_length, FILE * dot_class){
    for(int i = 0; i < local_variable_table_length; i++){
        local_variable_table->start_pc = u2Read(dot_class);
        local_variable_table->length = u2Read(dot_class);
        local_variable_table->name_index = u2Read(dot_class);
        local_variable_table->descriptor_index = u2Read(dot_class);
        local_variable_table->index = u2Read(dot_class);
    }
}
