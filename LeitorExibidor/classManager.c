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
        class_file->constant_pool[i].tag = u1Read(dot_class);
        printf("\nConstant Pool Tag: %d\n",class_file->constant_pool[i].tag);

        switch(class_file->constant_pool[i].tag){ //Dependendo do valor da TAG, será utilizado uma diferente struct da Union do Constant Pool
            case CONSTANT_Class:
                class_file->constant_pool[i].Class.name_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool[i].Class.name_index);
                break;
            case CONSTANT_Fieldref:
                class_file->constant_pool[i].Fieldref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].Fieldref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool[i].Fieldref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool[i].Fieldref.name_and_type_index);
                break;
            case CONSTANT_Methodref:
                class_file->constant_pool[i].Methodref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].Methodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool[i].Methodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool[i].Methodref.name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                class_file->constant_pool[i].InterfaceMethodref.class_index = u2Read(dot_class);
                class_file->constant_pool[i].InterfaceMethodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool[i].InterfaceMethodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool[i].InterfaceMethodref.name_and_type_index);
                break;
            case CONSTANT_String:
                class_file->constant_pool[i].String.string_index = u2Read(dot_class);
                    printf("\tSTRING_INDEX: %d\n",class_file->constant_pool[i].String.string_index);
                break;
            case CONSTANT_Integer:
                class_file->constant_pool[i].Integer.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool[i].Integer.bytes);
                break;
            case CONSTANT_Float:
                class_file->constant_pool[i].Float.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool[i].Float.bytes);
                break;
            case CONSTANT_Long:
                class_file->constant_pool[i].Long.high_bytes = u4Read(dot_class);
                class_file->constant_pool[i].Long.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool[i].Long.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool[i].Long.low_bytes);
                break;
            case CONSTANT_Double:
                class_file->constant_pool[i].Double.high_bytes = u4Read(dot_class);
                class_file->constant_pool[i].Double.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool[i].Double.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool[i].Double.low_bytes);
                break;
            case CONSTANT_NameAndType:
                class_file->constant_pool[i].NameAndType.name_index = u2Read(dot_class);
                class_file->constant_pool[i].NameAndType.descriptor_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool[i].NameAndType.name_index);
                    printf("\tDESCRIPTOR_INDEX: %d\n",class_file->constant_pool[i].NameAndType.descriptor_index);
                break;
            case CONSTANT_Utf8:
                    class_file->constant_pool[i].UTF8.length = u2Read(dot_class);
                        printf("\tLENGTH: %d\n",class_file->constant_pool[i].UTF8.length);
                        class_file->constant_pool[i].UTF8.bytes = (u1 *)malloc((class_file->constant_pool[i].UTF8.length * sizeof(u1)) + 1);
                    for(int j = 0; j < class_file->constant_pool[i].UTF8.length; j++){
                        class_file->constant_pool[i].UTF8.bytes[j] = u1Read(dot_class);
                            printf("\t\tBYTES: 0x%04x\n",class_file->constant_pool[i].UTF8.bytes[j]);
                    }
                    class_file->constant_pool[i].UTF8.bytes[class_file->constant_pool[i].UTF8.length] = '\0';
                    printf("\t\tBYTES CHAR: %s\n",class_file->constant_pool[i].UTF8.bytes);
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
        class_file->interfaces[i] = u2Read(dot_class);
            printf("\tINTERFACES: %d\n",class_file->interfaces[i]);
    }

    class_file->fields_count = u2Read(dot_class);
    printf("Fields Counter: %d\n",class_file->fields_count);
    class_file->fields = (field_info *)malloc(class_file->fields_count * sizeof(field_info));
   //system("pause");

    for(int i = 0; i < class_file->fields_count; i++){
        class_file->fields[i].access_flags = u2Read(dot_class);
        class_file->fields[i].name_index = u2Read(dot_class);
        class_file->fields[i].descriptor_index = u2Read(dot_class);
        class_file->fields[i].attributes_count = u2Read(dot_class);

        printf("\tACCESS_FLAGS: 0x%04x\n",class_file->fields[i].access_flags);
        printf("\tNAME_INDEX: 0x%04x\n",class_file->fields[i].name_index);
        printf("\tDESCRIPTOR_INDEX: 0x%04x\n",class_file->fields[i].descriptor_index);
        printf("\tATTRIBUTES_COUNT: 0x%04x\n",class_file->fields[i].attributes_count);
    }

    system("pause");
    class_file->methods_count = u2Read(dot_class);
    printf("Methods Counter: %d\n",class_file->methods_count);
    class_file->methods = (method_info *)malloc(class_file->methods_count * sizeof(method_info));

    for(int i = 0; i < class_file->methods_count; i++){
        class_file->methods[i].access_flags = u2Read(dot_class);
            printf("\tACCESS_FLAGS: 0x%04x\n",class_file->methods[i].access_flags);
        class_file->methods[i].name_index = u2Read(dot_class);
            printf("\tNAME_INDEX: 0x%04x\n",class_file->methods[i].name_index);
        class_file->methods[i].descriptor_index = u2Read(dot_class);
            printf("\tDESCRIPTOR_INDEX: 0x%04x\n",class_file->methods[i].descriptor_index);
        class_file->methods[i].attributes_count = u2Read(dot_class);
            printf("\tATTRIBUTES_COUNT: 0x%04x\n",class_file->methods[i].attributes_count);
        class_file->methods[i].attributes = (attribute_info *)malloc(class_file->methods[i].attributes_count * sizeof(attribute_info));

        readAttributesInfo(class_file->methods[i].attributes,class_file->methods[i].attributes_count,dot_class); //Chama a função que carrega as informações dos atributos
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
            printf("\tNAME_INDEX: %d\n",attributes->name_index);
        attributes->length = u4Read(dot_class);
            printf("\tLENGTH: %d\n",attributes->length);
        attributes->tag = u1Read(dot_class);
            printf("\tTAG: %d\n",attributes->tag);
        switch(attributes->tag){
            case ATTRIBUTE_ConstantValue:
                attributes->ConstantValue.index = u2Read(dot_class);
                    printf("\t\tConstantValue.index: %d\n",attributes->ConstantValue.index);
                break;
            case ATTRIBUTE_Code:
                attributes->Code.max_stack = u2Read(dot_class);
                    printf("\t\tCode.max_stack: %d\n",attributes->Code.max_stack);
                attributes->Code.max_locals = u2Read(dot_class);
                    printf("\t\tCode.max_locals: %d\n",attributes->Code.max_locals);
                attributes->Code.code_length = u4Read(dot_class);
                    printf("\t\tCode.code_length: %d\n",attributes->Code.code_length);
                attributes->Code.code = (u1 *)malloc((attributes->Code.code_length * sizeof(u1))+1);
                for(int k =0; k < attributes->Code.code_length; k++){
                    attributes->Code.code[k] =  u1Read(dot_class);
                }
                attributes->Code.code[attributes->Code.code_length] =  '\0';
                    printf("\t\tCode.code: %s\n",attributes->Code.code);
                attributes->Code.exception_table_length = u2Read(dot_class);
                    printf("\t\tCode.exception_table_length: %d\n",attributes->Code.exception_table_length);
                attributes->Code.ExceptionTable = (ExceptionTable *)malloc(attributes->Code.exception_table_length * sizeof(ExceptionTable));
                for(int k =0; k < attributes->Code.exception_table_length; k++){
                    attributes->Code.ExceptionTable[k].start_pc =  u2Read(dot_class);
                        printf("\t\tCode.ExceptionTable[%d].start_pc: %d\n",k,attributes->Code.ExceptionTable[k].start_pc);
                    attributes->Code.ExceptionTable[k].end_pc =  u2Read(dot_class);
                        printf("\t\tCode.ExceptionTable[%d].end_pc: %d\n",k,attributes->Code.ExceptionTable[k].end_pc);
                    attributes->Code.ExceptionTable[k].handler_pc =  u2Read(dot_class);
                        printf("\t\tCode.ExceptionTable[%d].handler_pc: %d\n",k,attributes->Code.ExceptionTable[k].handler_pc);
                    attributes->Code.ExceptionTable[k].catch_type =  u2Read(dot_class);
                        printf("\t\tCode.ExceptionTable[%d].catch_type: %d\n",k,attributes->Code.ExceptionTable[k].catch_type);
                }
                attributes->Code.attributes_count = u2Read(dot_class);
                    printf("\t\tCode.attributes_count: %d\n",attributes->Code.attributes_count);
                attributes->Code.attributes = (attribute_info *)malloc(attributes->Code.attributes_count * sizeof(attribute_info));
                readAttributesInfo(attributes->Code.attributes,attributes->Code.attributes_count,dot_class);
                break;
            case ATTRIBUTE_Exceptions:
                attributes->Exceptions.number_of_exceptions = u2Read(dot_class);
                    printf("\t\tExceptions.number_of_exceptions: %d\n",attributes->Exceptions.number_of_exceptions);
                attributes->Exceptions.exception_index_table = (u2 *)malloc(attributes->Exceptions.number_of_exceptions * sizeof(u2));
                for(int k =0; k < attributes->Exceptions.number_of_exceptions; k++){
                    attributes->Exceptions.exception_index_table[k] =  u2Read(dot_class);
                        printf("\t\tExceptions.exception_index_table[%d]: %d\n",k,attributes->Exceptions.exception_index_table[k]);
                }
                break;
            case ATTRIBUTE_InnerClasses:
                attributes->InnerClasses.number_of_classes = u2Read(dot_class);
                    printf("\t\tInnerClasses.number_of_classes: %d\n",attributes->InnerClasses.number_of_classes);
                attributes->InnerClasses.Classes = (Classes *)malloc(attributes->InnerClasses.number_of_classes * sizeof(Classes));
                readClasses(attributes->InnerClasses.Classes,attributes->InnerClasses.number_of_classes,dot_class); //Chama a função responsavel por ler os dados das Classes
                break;
            case ATTRIBUTE_Syntethic:
                break;
            case ATTRIBUTE_LineNumber:
                attributes->LineNumber.line_number_table_length = u2Read(dot_class);
                    printf("\t\tLineNumber.line_number_table_length: %d\n",attributes->LineNumber.line_number_table_length);
                attributes->LineNumber.Line_number_table = (Line_number_table *)malloc(attributes->LineNumber.line_number_table_length * sizeof(Line_number_table));
                readLineNumberTable(attributes->LineNumber.Line_number_table,attributes->LineNumber.line_number_table_length,dot_class);
                break;
            case ATTRIBUTE_LocalVariable:
                attributes->LocalVariable.local_variable_table_length = u2Read(dot_class);
                    printf("\t\tLocalVariable.local_variable_table_length: %d\n",attributes->LocalVariable.local_variable_table_length);
                attributes->LocalVariable.Local_variable_table = (Local_variable_table *)malloc(attributes->LocalVariable.local_variable_table_length * sizeof(Local_variable_table));
                readLocalVariableTable(attributes->LocalVariable.Local_variable_table,attributes->LocalVariable.local_variable_table_length,dot_class);
                break;
            case ATTRIBUTE_SourceFile:
                attributes->SourceFile.sourcefile_index = u2Read(dot_class);
                    printf("\t\tSourceFile.sourcefile_index: %d\n",attributes->SourceFile.sourcefile_index);
                break;
            default: //ATTRIBUTE_Unknown
                break;
        }
        system("pause");
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
